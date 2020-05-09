/*
  Overworld.cpp
  Author : Cyrielle
  File under GNU GPL v3.0 license
*/
#include "OverworldData.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>

#include "src/nlohmann/json.hpp"
#include "src/utils/OpString.hpp"
#include "src/utils/log.hpp"
#include "src/opmon/core/system/path.hpp"
#include "src/utils/ResourceLoader.hpp"
#include "src/opmon/core/Player.hpp"
#include "src/opmon/core/UiData.hpp"
#include "src/opmon/model/Move.hpp"
#include "src/opmon/model/Enums.hpp"
#include "src/opmon/model/Nature.hpp"
#include "src/opmon/model/OpMon.hpp"
#include "src/opmon/model/OpTeam.hpp"
#include "src/opmon/view/elements/Map.hpp"

namespace OpMon {

    OverworldData::OverworldData(UiData *uidata, Player *player)
        : uidata(uidata)
        , player(player)
        , gameMenuData(uidata, player) {

        using namespace Utils;

        Move::initMoves(std::filesystem::directory_iterator(Path::getResourcePath() + "data/moves"));

        player->addOpToOpTeam(new OpMon("", uidata->getOp(4), 5, {Move::newMove("Tackle"), Move::newMove("Growl"), nullptr, nullptr}, Nature::QUIET));

        //PP texture and rect loading
        Utils::ResourceLoader::load(texturePP, "sprites/chara/pp/pp_anim.png");
        texturePPRect[(unsigned int)Side::TO_DOWN] = sf::IntRect(0, 64, 32, 32);
        texturePPRect[(unsigned int)Side::TO_RIGHT] = sf::IntRect(32, 64, 32, 32);
        texturePPRect[(unsigned int)Side::TO_LEFT] = sf::IntRect(64, 64, 32, 32);
        texturePPRect[(unsigned int)Side::TO_UP] = sf::IntRect(96, 64, 32, 32);
        walkingPPRect[(unsigned int)Side::TO_DOWN] = sf::IntRect(0, 0, 32, 32);
        walkingPPRect[(unsigned int)Side::TO_RIGHT] = sf::IntRect(32, 0, 32, 32);
        walkingPPRect[(unsigned int)Side::TO_LEFT] = sf::IntRect(64, 0, 32, 32);
        walkingPPRect[(unsigned int)Side::TO_UP] = sf::IntRect(96, 0, 32, 32);
        walkingPP2Rect[(unsigned int)Side::TO_DOWN] = sf::IntRect(0, 32, 32, 32);
        walkingPP2Rect[(unsigned int)Side::TO_RIGHT] = sf::IntRect(32, 32, 32, 32);
        walkingPP2Rect[(unsigned int)Side::TO_LEFT] = sf::IntRect(64, 32, 32, 32);
        walkingPP2Rect[(unsigned int)Side::TO_UP] = sf::IntRect(96, 32, 32, 32);

        //Characters' textures initialization
        Utils::ResourceLoader::loadTextureArray(eventsTextures["kid"], "sprites/chara/kid/kid%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["fisherman"], "sprites/chara/fisherman/fisherman%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["kiwai"], "sprites/chara/prof/prof%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["playermom"], "sprites/chara/mom/mom%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["sk"], "sprites/chara/rival/sk%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["inferm"], "sprites/chara/inferm/inferm%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["|_| -|- |-| |= |_| N"], "sprites/chara/beta/alphabeta/otheon%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["beta"], "sprites/chara/beta/beta%d.png", 12);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["albd"], "sprites/chara/albd/albd%d.png", 12);

        //Initialization of doors
        Utils::ResourceLoader::loadTextureArray(eventsTextures["shop door"], "animations/shopdoor/shop_door%d.png", 4, 1);
        Utils::ResourceLoader::loadTextureArray(eventsTextures["door"], "animations/basicdoor/basic_door%d.png", 4, 1);

        eventsTextures.emplace("alpha", alphaTab);

        //Initialization of animated elements
        elementsCounter["windturbine"] = 0;
        elementsPos["windturbine"] = sf::Vector2f(8 * 32 + 25 * 32 - 7, 3 * 32 + 15);

        Utils::ResourceLoader::loadTextureArray(elementsTextures["windturbine"], "animations/windturbine/blade_%d.png", 16, 1);

        elementsCounter["smoke"] = 0;
        elementsPos["smoke"] = sf::Vector2f(8 * 32 + 18 * 32, 11 * 32);

        Utils::ResourceLoader::loadTextureArray(elementsTextures["smoke"], "animations/chimneysmoke/chimneysmoke_%d.png", 32, 1);

        //Items initialisation
        for(std::filesystem::directory_entry const& file : std::filesystem::directory_iterator(Path::getResourcePath() + "data/items")) {
        	if(file.is_regular_file()){
        		nlohmann::json itemsJson;

        		std::ifstream itemsJsonFile(file.path());

        		if(!itemsJsonFile) {
        			throw Utils::LoadingException("items.json", true);
        		}

        		itemsJsonFile >> itemsJson;

        		for(auto itor = itemsJson.begin(); itor != itemsJson.end(); ++itor) {
        			std::vector<std::unique_ptr<ItemEffect>> effects; //0 is opmon, 1 is player, 2 is held
        			for(auto eitor = itor->at("effects").begin(); eitor != itor->at("effects").end(); ++eitor) {
        				if(eitor->at("type") == "HpHealEffect") {
        					effects.push_back(std::make_unique<Items::HpHealEffect>(eitor->at("healed")));
        				} else {
        					effects.push_back(nullptr);
        				}
        			}
        			std::string itemId = itor->at("id");
        			itemsList.emplace(itemId, std::make_unique<Item>(Utils::OpString(uidata->getStringKeys(), "items." + itemId + ".name"), itor->at("usable"), itor->at("onOpMon"), std::move(effects[0]), std::move(effects[1]), std::move(effects[2])));
        		}
        	}
        }

        for(std::filesystem::directory_entry const& file : std::filesystem::directory_iterator(Path::getResourcePath() + "data/trainers")) {
        	if(file.is_regular_file()){
        		std::ifstream trainersFile(file.path());
        		nlohmann::json trainersJson;
        		trainersFile >> trainersJson;

        		for(auto itor = trainersJson.begin(); itor != trainersJson.end(); ++itor) {
        			OpTeam *team = new OpTeam(itor->at("name"));
        			for(auto opmonItor = itor->at("team").begin(); opmonItor != itor->at("team").end(); ++opmonItor) {
        				team->addOpMon(new OpMon(opmonItor->at("nickname"),
        						uidata->getOp(opmonItor->at("species")),
								opmonItor->at("level"),
								{Move::newMove(opmonItor->at("moves")[0]),
										Move::newMove(opmonItor->at("moves")[1]),
										Move::newMove(opmonItor->at("moves")[2]),
										Move::newMove(opmonItor->at("moves")[3])},
										opmonItor->at("nature")));
        			}
        			trainers.emplace(itor->at("name"), team);
        			std::string strName = itor->at("name");
        			Utils::Log::oplog("Loaded trainer " + strName);
        		}
        	}
        }

        completions.emplace("playername", player->getNameP());

        //Maps loading
        for(std::filesystem::directory_entry const& file : std::filesystem::directory_iterator(Path::getResourcePath() + "data/maps")) { //One map per JSON file
        	if(file.is_regular_file()){
        		nlohmann::json mapJson;
        		std::ifstream mapFile(file.path());
        		mapFile >> mapJson;
        		maps.emplace(mapJson.at("id"), new Elements::Map(mapJson));
        	}
        }

        mapsItor = maps.begin();
    }

    OverworldData::~OverworldData() {
        for(auto &map : maps) {
            delete(map.second);
        }
        delete(player);
    }

    Elements::Map *OverworldData::getMap(std::string const &map) {
        if(!maps[map]->isLoaded()) {
            Elements::Map *newMap = maps[map]->loadMap(*this);
            delete(maps[map]);
            maps[map] = newMap;
        }
        return maps[map];
    }

    Elements::Map *OverworldData::getCurrentMap() {
        return getMap(player->getMapId());
    }

    std::vector<sf::Texture> &OverworldData::getEventsTexture(std::string const &key) { //Uncomment commented lines when C++20 is commonly used
        //#if __cplusplus > 201703L
    	//if(!eventsTextures.contains(key)){
        //#else
    	bool contains = false;
    	for(std::pair<std::string, std::vector<sf::Texture> > pair : eventsTextures){
    		if(pair.first == key) {
    			contains = true;
    			break;
    		}
    	}
    	if(!contains){
            //#endif
    		Utils::Log::warn("Event texture key " + key + " not found. Returning alpha.");
    		return eventsTextures["alpha"];
    	}else return eventsTextures[key];
    }

} // namespace OpMon
