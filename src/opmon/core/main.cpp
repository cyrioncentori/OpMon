/*
main.cpp
Author : Cyrielle
Contributors : Stelyus, Navet56
File under GNU GPL v3.0 license
*/
#include <time.h>
#include <string>
#include <iostream>
#include <map>

#include "../../utils/defines.hpp"
#include "../../utils/fs.hpp"
#include "../../utils/log.hpp"
#include "../../utils/path.hpp"
#include "../../utils/time.hpp"
#include "system/OptionsSave.hpp"
#include "system/ResourceLoader.hpp"
#include "Gameloop.hpp"
#include "i18n/Translator.hpp"
#include "config.hpp"
#include "src/opmon/core/Core.hpp"

using Utils::Log::oplog;

//Compilation with Visual Studio
// MSC uses WinMain() instead of main().
#ifdef _MSC_VER

#include <windows.h>

int main(int argc, char *argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

//#define DEBUG

namespace OpMon {
    namespace Main {

        std::string version = OPMON_VERSION;

        std::string pre = "0";

        std::string versionS;

        int starts() {

            oplog("Log opening OK. Welcome in OpMon Lazuli.");
            oplog("Version : Alpha " + version + ((pre == "0") ? "" : ("-pre_" + pre)));
            std::ostringstream osslog;
            osslog << "Date in seconds : " << time(NULL);
            oplog(osslog.str());
#ifdef _WIN32
            oplog("Plateform : Windows");
#else
            oplog("Plateform : Unix");
#endif
            oplog("Loading options");
            System::OptionsSave::initParams(Utils::Path::getSavePath() + "/optSave.oparams"); //Loading parameters
            if(!System::OptionsSave::checkParam("lang")) {                                    //If the "lang" setting don't exist
                System::OptionsSave::addParam("lang", "eng");
            }
            oplog("Resources repertory : " + Utils::Path::getResourcePath());

            //Initializaing keys
            oplog("Loading strings");
            std::string lang = System::OptionsSave::getParam("lang").getValue();
            auto &tr = I18n::Translator::getInstance();

            if(!tr.getAvailableLanguages().count(lang)) {
                lang = "en"; // The lang isn't available. Default to english.
            }
            tr.setLang(lang);

            oplog("Loading completed! Opening gui.");

            bool reboot = false;
            do {
                reboot = false;
                oplog("Starting game loop");

                GameLoop gameloop;
                reboot = gameloop() == GameStatus::REBOOT;

                std::ostringstream logEntry;
                logEntry << std::string("Game ended after ") << Utils::Time::getElapsedSeconds() << std::string("seconds");

                oplog(logEntry.str());
                if(reboot) {
                    oplog("Restarting the game.");
                }
            } while(reboot);
            oplog("Ending the game normally.");
            return quit(0);
        }
    } // namespace Main
} // namespace OpMon

int main(int argc, char *argv[]) {
    Utils::Time::initClock();

    auto versionS = "Version : Alpha " + OpMon::Main::version + ((OpMon::Main::pre == "0") ? "" : ("-pre_" + OpMon::Main::pre));

    if(!OpMon::System::ResourceLoader::checkResourceFolderExists() || !Utils::Fs::mkdir(SAVE_PATH)) {
        std::cout << "Exiting" << std::endl;
        return -1;
    }

    //Checking parameters
    if(argc >= 2) {
        for(int i = 0; i < argc; i++) {
            std::string str = std::string(argv[i]);
            if(str == "--version") {
                std::cout << "OpMon Lazuli version " << versionS << std::endl;
                std::cout << "Under GNU GPL v3.0 license" << std::endl;
                std::cout << "http://opmon-game.ga" << std::endl;
                return 0;
            } else if(str == "--help") {
                std::cout << "--version : Prints the version and quit." << std::endl;
                std::cout << "--help : Prints this message and quit." << std::endl;
                std::cout << "--debug : Starts the game with debug code. Changes when needed." << std::endl;
                return 0;
            }
        }
    }
    return OpMon::Main::starts();
}