#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <tmx/MapLoader.h>
#include <iostream>
#include <vector>
#include <list>
using namespace sf;

class Entity;
class GameView;
class SF_Desk;
class SFGUI_Entity;
class Player;
class Skill;
class SFGUI_Window;
class FireBall;
class Footer;
class MainMenu;
class OptionMenu;
enum skillType{ otherType, noImage, musicOn, musicOff, webThrow, strength, silence, horror, gainArmor, sword, lance, fireBall, fireBurst, purpleBall };

#include "Load_Singleton.h"
#include "PathFinder.h"
#include "Properties.h"