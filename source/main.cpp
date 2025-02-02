
#include "net/server.hpp"
#include "util/time.hpp"
#include "wrappers/driver.hpp"
#include "wrappers/openGL.hpp"

#include <csignal>

static bool run{ true };

void signalHandler(int dummy)
{
    if (not run) {
        std::exit(-1);
    }
    run = false;
    printf("Quitting\n");
}

int main(int argc, char **argv)
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    //Driver driver(Driver::Mode::SH1106_128x64);
    Wrappers::Driver driver(Wrappers::Driver::Mode::SSD1322);

    uint8_t glBuffer[256 * 64 * 3];

    int frameCount{};

    Wrappers::OpenGL glWrapper(driver.GetWidth(), driver.GetHeight());
    ResourceHandler resHandler(driver.GetWidth(), driver.GetHeight());
    AnimationController animation(driver.GetWidth(), driver.GetHeight());

    //animation.AddTriangle(-0.5f, 0.5f);
    animation.AddText("Christophe", true);
    animation.AddSprite("res/red-panda-transparent.png", true);

    auto current = std::chrono::steady_clock::now();
    auto prev    = std::chrono::steady_clock::now();

    long long int totalFrameTimes{};

    decltype(current) sectionTimes[4];
    long long int sectionDeltas[3]{ 0, 0, 0 };

    int64_t startTime{ util::timing::Get() };

    float now{};

    WebServer server(animation);
    std::thread serverThread([&server]() {
        server.Run();
    });

    while (run) {
        now = static_cast<float>(static_cast<double>(util::timing::Get() - startTime) / static_cast<double>(util::timing::Frequency()));

        animation.ProcessRequests();

        sectionTimes[0] = std::chrono::steady_clock::now();

        // section 1: openGL operations
        glWrapper.PreDraw();
        animation.Draw(now);

        sectionTimes[1] = std::chrono::steady_clock::now();

        // section 2: output buffer conversion and transfer
        // expected to be constant
        glWrapper.PostDraw(glBuffer);
        driver.CopyGLBuffer(glBuffer);

        sectionTimes[2] = std::chrono::steady_clock::now();

        // section 3: clear buffer, wipe screen
        // expected to be constant
        driver.Display();
        driver.Clear();

        sectionTimes[3] = std::chrono::steady_clock::now();

        current = std::chrono::steady_clock::now();

        //printf("Frametime: %llims\n", delta.count());
        totalFrameTimes += std::chrono::duration_cast<std::chrono::milliseconds>(current - prev).count();
        for (int i = 0; i < 3; i++) {
            auto timeDifference{ std::chrono::duration_cast<std::chrono::microseconds>(sectionTimes[i + 1] - sectionTimes[i]).count() };
            sectionDeltas[i] += timeDifference;
        }
        frameCount++;
        std::swap(current, prev);
    }

    server.Halt();
    serverThread.join();

    printf("Avarage timing:\n");
    printf("Total time:           %07.3lfms\n", static_cast<double>(totalFrameTimes) / frameCount);
    printf("Section 1 (gl)  :   %09.3lfµs\n", static_cast<double>(sectionDeltas[0]) / frameCount);
    printf("Section 2 (copy):   %09.3lfµs\n", static_cast<double>(sectionDeltas[1]) / frameCount);
    printf("Section 3 (cls) :   %09.3lfµs\n", static_cast<double>(sectionDeltas[2]) / frameCount);
}
