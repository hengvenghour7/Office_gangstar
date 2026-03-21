#include "map.h"
#include "globalVar.h"
#include "helpers.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>

using json = nlohmann::json;

WorldDrawProperty::WorldDrawProperty (int width, int height, std::vector<int>* collisionData) : collisionArray(arrayTo2DArray(collisionData, width)) {
};
void WorldDrawProperty::changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData) {
    
};
TrafficLightProp::TrafficLightProp (const char * imgSrc, int id, Rectangle dimension, Rectangle srcDimension): lightTexture(LoadTexture(imgSrc)),
    id(id), dimension(dimension), srcDimension(srcDimension) {

}
void TrafficLightProp::drawTrafficLight (Vector2 mapPos, int currentFrame) {
    Rectangle recDes = {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height};
    srcDimension.x = currentFrame * srcDimension.width;
    DrawTexturePro(lightTexture, srcDimension, recDes, {0, 0}, 0, WHITE);
}
TrafficLightSet::TrafficLightSet (Rectangle dimension, int id, TrafficLightState startingLightState) : collision(dimension), id(id),
    lightState(startingLightState), countdownTime(0), currentFrame(getCurrentFrameByLightState(startingLightState)), maxRedFrame(3), maxYellowFrame(6),
    maxGreenFrame(9), isTransition(false) {

}
int TrafficLightSet::getCurrentFrameByLightState (TrafficLightState inputLightState) {
    switch (inputLightState)
    {
    case TrafficLightState::Red:
        return 1;
    case TrafficLightState::Yellow:
        return 4;
    case TrafficLightState::Green:
        return 7;
    default:
        return 1;
    }
}
void TrafficLightSet::tick (float deltaTime) {
    if (isTransition) {
        if (countdownTime >= 0.1) {
            countdownTime = 0;
            changeLight();
        }
    } else {
        if (countdownTime >= timeToNextlight) {
            countdownTime = 0;
            changeLight();
        }
    }
    countdownTime += deltaTime;
}
void TrafficLightSet::changeLight () {
    int temp_light_state = (int)lightState;
    currentFrame--;
    if (currentFrame != (redActiveFrame) && currentFrame != (yellowActiveFrame) && currentFrame != (greenActiveFrame)) {
        isTransition = true;
    } else {
        isTransition = false;
    }
    if (currentFrame < 9 && currentFrame % 3 == 0) temp_light_state --;
    if (currentFrame <= 0) {
        currentFrame = maxGreenFrame;
        temp_light_state = (int)TrafficLightState::Green;
    }
    if (temp_light_state < (int)TrafficLightState::Red) {
        temp_light_state = (int)TrafficLightState::Green;
    }
    switch (temp_light_state)
    {
    case (int)TrafficLightState::Red:
        timeToNextlight = redFrameDuration;
        break;
    case (int)TrafficLightState::Yellow:
        timeToNextlight = yellowFrameDuration;
        break;
    case (int)TrafficLightState::Green:
        timeToNextlight = greenFrameDuration;
        break;
    default:
        timeToNextlight = greenFrameDuration;
        break;
    }
    lightState = (TrafficLightState)temp_light_state;
}
void TrafficLightSet::draw (Vector2 mapPos) {
    for (TrafficLightProp prop : trafficLightProps) {
        prop.drawTrafficLight(mapPos, currentFrame);
    }
}
Rectangle TrafficLightSet::getCollision () {
    return collision;
}
TrafficLightState TrafficLightSet::getLightState () {
    return lightState;
}
void TrafficLightSet::AddTrafficLightProp (std::string imgSrc, int id, Rectangle dimension, int srcWidth, int srcHeight) {
    const char *temp_imgSrc = imgSrc.c_str();
    Rectangle srcDimension = {(float)currentFrame, 0, (float)srcWidth, (float)srcHeight};
    trafficLightProps.emplace_back(temp_imgSrc, id, dimension, srcDimension);
}
Shop::Shop(Rectangle shopDimension, std::string name, std::vector<ShopItemProperties> allShopItemProperties): 
    shopDimension(shopDimension),
    name(name) {
        for (ShopItemProperties property: allShopItemProperties) {
            shopItems.emplace_back(property.textureSrc, property.name, property.heal, property.energyHeal, property.price, property.description);
        }
};
std::vector<ShopItem>* Shop::getShopItems(){
    return &shopItems;
}
std::string Shop::getShopName() {
    return name;
}
Rectangle Shop::getShopDimension(Vector2 mapPos) {
    Rectangle screenShopDimension = {shopDimension.x + mapPos.x, shopDimension.y + mapPos.y, shopDimension.width, shopDimension.height};
    return screenShopDimension;
}
InteractablePropV2::InteractablePropV2 (Rectangle dimension, std::function<void()> function, std::string imgSrc, 
        int startFrame, int midFrame, int endFrame, int srcWidth, int srcHeight, int interactableDistance, int row,
        int srcYOffset): 
    dimension(dimension), func(function), imgTexture(LoadTexture(imgSrc.c_str())), currentFrame(0), startFrame(startFrame), midFrame(midFrame), endFrame(endFrame),
    srcWidth(srcWidth), srcHeight(srcHeight), interactableDistance(interactableDistance), row(row), srcYOffset(srcYOffset) {

}
Vector2 InteractablePropV2::getCenter(Vector2& mapPos) {
    Vector2 center = {dimension.x + mapPos.x + dimension.width*0.5f,dimension.y + mapPos.y + dimension.height*0.5f};
    return center;
}
void InteractablePropV2::doFunction() {
    func();
}
Rectangle InteractablePropV2::getDimension() {
    return dimension;
}
void InteractablePropV2::draw (Vector2 mapPos) {
    Rectangle srcDimension = {(float)currentFrame * srcWidth, row + (float)srcYOffset, (float)srcWidth, (float)srcHeight};
    DrawTexturePro(imgTexture, srcDimension, 
        {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height},
        {0, 0}, 0, WHITE
    );
}
void InteractablePropV2::updateAnimation (float deltaTime) {
    if (isContinueAnimation) {
        if (!isInteracted) {
            if (animationUpdateTime > 0.1) {
                animationUpdateTime = 0;
                if (currentFrame < midFrame) {
                    currentFrame++;
                } else {
                    isActionFinished = true;
                    isContinueAnimation = false;
                    isInteracted = true;
                    // currentFrame = startFrame;
                }
            }
        } else {
            if (animationUpdateTime > 0.1) {
                animationUpdateTime = 0;
                if (currentFrame < endFrame) {
                    currentFrame++;
                } else {
                    isActionFinished = true;
                    isContinueAnimation = false;
                    isInteracted = false;
                    currentFrame = startFrame;
                }
            }
        }
        animationUpdateTime += deltaTime;
    }
}
void InteractablePropV2::handleInteraction () {
    if (isActionFinished) {
        isActionFinished = false;
        isContinueAnimation = true;
        doFunction();
    }
}
Car::Car (Rectangle dimension, std::function<void()> function, std::string imgSrc, 
        int startFrame, int midFrame, int endFrame, int srcWidth, int srcHeight, int interactableDistance, int row,
        int srcYOffset, MovementFrameSet movementFrameSet): 
        InteractablePropV2
        (
            dimension, function, imgSrc, startFrame, midFrame, endFrame, srcWidth, srcHeight, 
            interactableDistance, row, srcYOffset
        ),
        movementFrameSet(movementFrameSet),
        drawDesDimension(dimension)
        {
            
        }
void Car::draw (Vector2 mapPos) {
    Rectangle srcDimension = {(float)currentFrame * srcWidth + srcXOffset, row * srcHeight + (float)srcYOffset, (float)srcWidth, (float)srcHeight};
    DrawTexturePro(imgTexture, srcDimension, 
        {drawDesDimension.x + mapPos.x, drawDesDimension.y + mapPos.y, drawDesDimension.width, drawDesDimension.height},
        {0, 0}, 0, WHITE
    );
};
void Car::setDirection (Vector2 direction) {
    this->direction = direction;
}
void Car::updateAnimation (float deltaTime) {
    if (animationUpdateTime > 0.1) {
        animationUpdateTime = 0;
        if (currentFrame < endFrame) {
            currentFrame++;
        } else {
            currentFrame = startFrame;
        }
    }
    switch (directionState)
    {
    case CarDirectionState::Left:
        row = movementFrameSet.leftMovementFrame.frameRow;
        srcHeight = movementFrameSet.leftMovementFrame.frameHeight;
        srcWidth = movementFrameSet.leftMovementFrame.frameWidth;
        srcXOffset = movementFrameSet.leftMovementFrame.XOffset;
        srcYOffset = movementFrameSet.leftMovementFrame.YOffset;
        startFrame = movementFrameSet.leftMovementFrame.startFrame;
        endFrame = movementFrameSet.leftMovementFrame.endFrame;
        drawDesDimension.x = dimension.x + dimension.width*0.5 - movementFrameSet.leftMovementFrame.desWidth*0.5 * MAP_SCALE;
        drawDesDimension.y = dimension.y + dimension.height*0.5 - movementFrameSet.leftMovementFrame.desHeight*0.5 * MAP_SCALE;
        drawDesDimension.width = movementFrameSet.leftMovementFrame.desWidth * MAP_SCALE;
        drawDesDimension.height = movementFrameSet.leftMovementFrame.desHeight * MAP_SCALE;
        break;
    case CarDirectionState::Right:
        row = movementFrameSet.rightMovementFrame.frameRow;
        srcHeight = movementFrameSet.rightMovementFrame.frameHeight;
        srcWidth = movementFrameSet.rightMovementFrame.frameWidth;
        srcXOffset = movementFrameSet.rightMovementFrame.XOffset;
        srcYOffset = movementFrameSet.rightMovementFrame.YOffset;
        startFrame = movementFrameSet.rightMovementFrame.startFrame;
        endFrame = movementFrameSet.rightMovementFrame.endFrame;
        drawDesDimension.x = dimension.x + dimension.width*0.5 - movementFrameSet.rightMovementFrame.desWidth*0.5 * MAP_SCALE;
        drawDesDimension.y = dimension.y + dimension.height*0.5 - movementFrameSet.rightMovementFrame.desHeight*0.5 * MAP_SCALE;
        drawDesDimension.width = movementFrameSet.rightMovementFrame.desWidth * MAP_SCALE;
        drawDesDimension.height = movementFrameSet.rightMovementFrame.desHeight * MAP_SCALE;
        break;
    case CarDirectionState::Up:
        row = movementFrameSet.upMovementFrame.frameRow;
        srcHeight = movementFrameSet.upMovementFrame.frameHeight;
        srcWidth = movementFrameSet.upMovementFrame.frameWidth;
        srcXOffset = movementFrameSet.upMovementFrame.XOffset;
        srcYOffset = movementFrameSet.upMovementFrame.YOffset;
        startFrame = movementFrameSet.upMovementFrame.startFrame;
        endFrame = movementFrameSet.upMovementFrame.endFrame;
        drawDesDimension.x = dimension.x + dimension.width*0.5 - movementFrameSet.upMovementFrame.desWidth*0.5 * MAP_SCALE;
        drawDesDimension.y = dimension.y + dimension.height*0.5 - movementFrameSet.upMovementFrame.desHeight*0.5 * MAP_SCALE;
        drawDesDimension.width = movementFrameSet.upMovementFrame.desWidth * MAP_SCALE;
        drawDesDimension.height = movementFrameSet.upMovementFrame.desHeight * MAP_SCALE;
        break;
    case CarDirectionState::Down:
        row = movementFrameSet.downMovementFrame.frameRow;
        srcHeight = movementFrameSet.downMovementFrame.frameHeight;
        srcWidth = movementFrameSet.downMovementFrame.frameWidth;
        srcXOffset = movementFrameSet.downMovementFrame.XOffset;
        srcYOffset = movementFrameSet.downMovementFrame.YOffset;
        startFrame = movementFrameSet.downMovementFrame.startFrame;
        endFrame = movementFrameSet.downMovementFrame.endFrame;
        drawDesDimension.x = dimension.x + dimension.width*0.5 - movementFrameSet.downMovementFrame.desWidth*0.5 * MAP_SCALE;
        drawDesDimension.y = dimension.y + dimension.height*0.5 - movementFrameSet.downMovementFrame.desHeight*0.5 * MAP_SCALE;
        drawDesDimension.width = movementFrameSet.downMovementFrame.desWidth * MAP_SCALE;
        drawDesDimension.height = movementFrameSet.downMovementFrame.desHeight * MAP_SCALE;
        break;
    default:
        break;
    }
    animationUpdateTime+= deltaTime;
    dimension.x += direction.x * speed;
    dimension.y += direction.y * speed;
};
void Car::findDrivingPath(std::vector<std::vector<int>>* pathArray) {
    findAllPath(pathArray, dimension, &direction);
    if (direction.y < 0) {
        directionState = CarDirectionState::Up;
        return;
    }
    if (direction.y > 0) {
        directionState = CarDirectionState::Down;
        return;
    }
    if (direction.x > 0) {
        directionState = CarDirectionState::Right;
        return;
    }
    if (direction.x < 0) {
        directionState = CarDirectionState::Left;
        return;
    }
    directionState = CarDirectionState::Right;
    dimension.x += direction.x;
    dimension.y += direction.y;
};
WorldSet::WorldSet(const char* backgroundTexture, const char* foregroundTexture, std::vector<DrawingDataSet> drawingDataSet, int mapWidth, int mapHeight, 
    std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath,
    WorldEnums worldName, Player& player, std::vector<InteractableInputProperties> interactableProperties, int levelAmount, int AI_amount): 
    drawProperty(mapWidth, mapHeight, collisionData), background(backgroundTexture, &drawProperty), foreground(foregroundTexture, &drawProperty), worldProps(worldProps),
    worldName(worldName),
    explosionTexture(LoadTexture("resources/image/animated/Landmine_Explosion.png"))  {
        for (DrawingDataSet obj: drawingDataSet) {
            mapLayers.emplace_back(obj.imgSrc, obj.level);
        }
        if (AI_amount > 0) {
            std::random_device rd;

            // Create a Mersenne Twister generator seeded with rd
            std::mt19937 gen(rd());

            std::uniform_real_distribution<float> dis(1.f, 4.0f);
            // Generate a random number
            float randomValue = dis(gen);
            for (int i = 0; i< AI_amount; i++) {
                AIPlayers.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4, dis(gen) + 20, getWorldCollisionArray());
            }
        }
        std::fstream file (mapPropertyPath);
    if (!file.is_open()) {
        std::cout<<"error, can't open file";
    } else {
        json j;
        file >> j;
        auto temp_interactable_items = getObjectsFromJsonLayer(j, "pickable_items", {"imageSrc", "textureWidth", "textureHeight"});
        for (ObjectDetail obj: temp_interactable_items) {
            interactableItemList.emplace_back(
                obj.getProperty("textureWidth").get<int>(),
                obj.getProperty("textureHeight").get<int>(),
                obj.getProperty("imageSrc").get<std::string>().c_str(),
                obj.getDimension());
        }
        auto temp_level_switchers = getObjectsFromJsonLayer(j, "level_switcher", {"option_1", "option_2"});
        for (int i =0 ; i < temp_level_switchers.size(); i++) {
            ObjectDetail obj = temp_level_switchers[i];
            autoLevelSwitcherList.emplace(i, 
                LevelSwitcher(obj.getProperty("option_1").get<int>(), obj.getProperty("option_2").get<int>(), obj.getDimension()));
        }
        if (levelAmount > 0) {
            for (int i = 0; i < levelAmount; i++) {
                std::vector<int> tmpDataArray = getArrayFromJson(j, "collision_lvl_" + std::to_string(i + 1));
                std::vector<std::vector<int>> tmpCollisionArray = arrayTo2DArray( &tmpDataArray, mapWidth);
                std::vector<int>::iterator tmpCollisionCode = std::find_if(tmpDataArray.begin(), tmpDataArray.end(), [](int data) {
                    return data != 0;
                });
                if (tmpCollisionCode != tmpDataArray.end()) {
                    levelDataList[i + 1] = {tmpCollisionArray, *tmpCollisionCode};
                }
            }
        }
        if (interactableProperties.size() > 0) {
            std::vector<ObjectDetail> temp_objs = getObjectsFromJsonLayer(j, "interactable_items", {"imgSrc", "name", 
                "interactableDistance", "startFrame", "midFrame", "endFrame", "srcWidth", "srcHeight", "row", "srcYOffset"});
            for (ObjectDetail obj : temp_objs) {
                std::string name = obj.getProperty("name").get<std::string>();
                for (InteractableInputProperties property : interactableProperties) {
                    if (name == property.name) {
                        std::string temp_imgSrc = obj.getProperty("imgSrc").get<std::string>();
                        int temp_startFrame = obj.getProperty("startFrame").get<int>();
                        int temp_midFrame = obj.getProperty("midFrame").get<int>();
                        int temp_endFrame = obj.getProperty("endFrame").get<int>();
                        int temp_srcWidth = obj.getProperty("srcWidth").get<int>();
                        int temp_srcHeight = obj.getProperty("srcHeight").get<int>();
                        int temp_row = obj.getProperty("row").get<int>();
                        int temp_srcYOffset = obj.getProperty("srcYOffset").get<int>();
                        int temp_interactableDistance = obj.getProperty("interactableDistance").get<int>();
                        interactableV2List.emplace_back(obj.getDimension(), property.func, temp_imgSrc, temp_startFrame, temp_midFrame, temp_endFrame,
                            temp_srcWidth, temp_srcHeight, temp_interactableDistance, temp_row, temp_srcYOffset);
                    }
                }
            }
        }
        {
            std::vector<ObjectDetail> temp_cars = getObjectsFromJsonLayer(j, "car_items", {"imgSrc", "name", 
                    "interactableDistance", "startFrame", "midFrame", "endFrame", "srcWidth", "srcHeight", "row", "srcYOffset",
                    "leftStartFrame", "leftEndFrame", "leftFrameRow", "leftFrameWidth", "leftFrameHeight", "leftFrameXOffset", "leftFrameYOffset", "leftDesWidth", "leftDesHeight",
                    "rightStartFrame", "rightEndFrame", "rightFrameRow", "rightFrameWidth", "rightFrameHeight", "rightFrameXOffset", "rightFrameYOffset", "rightDesWidth", "rightDesHeight",
                    "upStartFrame", "upEndFrame", "upFrameRow", "upFrameWidth", "upFrameHeight", "upFrameXOffset", "upFrameYOffset", "upDesWidth", "upDesHeight",
                    "downStartFrame", "downEndFrame", "downFrameRow", "downFrameWidth", "downFrameHeight", "downFrameXOffset", "downFrameYOffset", "downDesWidth", "downDesHeight"});
            for (ObjectDetail carObj : temp_cars) {
                    std::string temp_imgSrc = carObj.getProperty("imgSrc").get<std::string>();
                    int temp_startFrame = carObj.getProperty("startFrame").get<int>();
                    int temp_midFrame = carObj.getProperty("midFrame").get<int>();
                    int temp_endFrame = carObj.getProperty("endFrame").get<int>();
                    int temp_srcWidth = carObj.getProperty("srcWidth").get<int>();
                    int temp_srcHeight = carObj.getProperty("srcHeight").get<int>();
                    int temp_row = carObj.getProperty("row").get<int>();
                    int temp_srcYOffset = carObj.getProperty("srcYOffset").get<int>();
                    MovementFrameSet temp_movementFrameSet = {
                        {
                            carObj.getProperty("leftStartFrame").get<int>(),
                            carObj.getProperty("leftEndFrame").get<int>(),
                            carObj.getProperty("leftFrameRow").get<int>(),
                            carObj.getProperty("leftFrameWidth").get<int>(),
                            carObj.getProperty("leftFrameHeight").get<int>(),
                            carObj.getProperty("leftFrameXOffset").get<int>(),
                            carObj.getProperty("leftFrameYOffset").get<int>(),
                            carObj.getProperty("leftDesWidth").get<int>(),
                            carObj.getProperty("leftDesHeight").get<int>(),
                        },
                        {
                            carObj.getProperty("rightStartFrame").get<int>(),
                            carObj.getProperty("rightEndFrame").get<int>(),
                            carObj.getProperty("rightFrameRow").get<int>(),
                            carObj.getProperty("rightFrameWidth").get<int>(),
                            carObj.getProperty("rightFrameHeight").get<int>(),
                            carObj.getProperty("rightFrameXOffset").get<int>(),
                            carObj.getProperty("rightFrameYOffset").get<int>(),
                            carObj.getProperty("rightDesWidth").get<int>(),
                            carObj.getProperty("rightDesHeight").get<int>(),
                        },
                        {
                            carObj.getProperty("upStartFrame").get<int>(),
                            carObj.getProperty("upEndFrame").get<int>(),
                            carObj.getProperty("upFrameRow").get<int>(),
                            carObj.getProperty("upFrameWidth").get<int>(),
                            carObj.getProperty("upFrameHeight").get<int>(),
                            carObj.getProperty("upFrameXOffset").get<int>(),
                            carObj.getProperty("upFrameYOffset").get<int>(),
                            carObj.getProperty("upDesWidth").get<int>(),
                            carObj.getProperty("upDesHeight").get<int>(),
                        },
                        {
                            carObj.getProperty("downStartFrame").get<int>(),
                            carObj.getProperty("downEndFrame").get<int>(),
                            carObj.getProperty("downFrameRow").get<int>(),
                            carObj.getProperty("downFrameWidth").get<int>(),
                            carObj.getProperty("downFrameHeight").get<int>(),
                            carObj.getProperty("downFrameXOffset").get<int>(),
                            carObj.getProperty("downFrameYOffset").get<int>(),
                            carObj.getProperty("downDesWidth").get<int>(),
                            carObj.getProperty("downDesHeight").get<int>(),
                        }
                    };
                    int temp_interactableDistance = carObj.getProperty("interactableDistance").get<int>();
                carList.emplace_back(carObj.getDimension(), []() {}, temp_imgSrc, temp_startFrame, temp_midFrame, temp_endFrame,
                            temp_srcWidth, temp_srcHeight, temp_interactableDistance, temp_row, temp_srcYOffset, temp_movementFrameSet);
            }
        }
        {
            std::vector<ObjectDetail> temp_traffic_light_sets = getObjectsFromJsonLayer(j, "traffic_light_detector", 
            {"id", "starting_light"});
            for (ObjectDetail set: temp_traffic_light_sets) {
                int temp_id = set.getProperty("id").get<int>();
                TrafficLightState temp_starting_light = (TrafficLightState)set.getProperty("starting_light").get<int>();
                Rectangle temp_dimension = set.getDimension();
                if (temp_starting_light > TrafficLightState::Green || temp_starting_light < TrafficLightState::Red)
                    temp_starting_light = TrafficLightState::Red;
                trafficLights.emplace(temp_id, TrafficLightSet(temp_dimension, temp_id, temp_starting_light));
                trafficLights.at(temp_id).setY(100*TILE_SIZE*MAP_SCALE + 2);
            }
            std::vector<ObjectDetail> temp_traffic_light_obj = getObjectsFromJsonLayer(j, "traffic_light", 
                {"id", "imgSrc", "srcHeight", "srcWidth"});
            for (ObjectDetail obj : temp_traffic_light_obj) {
                int temp_id = obj.getProperty("id");
                std::string temp_imgSrc = obj.getProperty("imgSrc").get<std::string>();
                int temp_srcHeight = obj.getProperty("srcHeight").get<int>();
                int temp_srcWidth = obj.getProperty("srcWidth").get<int>();
                Rectangle temp_dimension = obj.getDimension();
                trafficLights.at(temp_id).AddTrafficLightProp(temp_imgSrc, temp_id, temp_dimension, temp_srcWidth, temp_srcHeight);
            }
        }
        if (carList.size() > 0) {
            std::vector<int> mapCollisionData = getArrayFromJson(j, "car_path");
            carPathArray = arrayTo2DArray(&mapCollisionData, mapWidth);
        }
        auto layers = j["layers"];
        std::vector<int>::iterator tempCollision = std::find_if(collisionData->begin(), collisionData->end(), [](int data) {
            return data != 0;
        });
        if (tempCollision != collisionData->end()) {
            collisionCode = *tempCollision != 0 ? *tempCollision : 1;
        }
        levelDataList[0] = {drawProperty.collisionArray, collisionCode};
        auto spawnPointLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "entry point";
            });
        if (spawnPointLayer != layers.end()) {
            for (auto &obj : (*spawnPointLayer)["objects"]) {
                int spawnIndex{};
                int spawnToIndex{};
                bool isAuto{false};
                SwitchDirectionEnum direction = SwitchDirectionEnum::Top;
                int width{(int)(obj["width"].get<float>()*MAP_SCALE)};
                int height{(int)(obj["height"].get<float>()*MAP_SCALE)};
                std::string inputSwitchToMap{};
                Vector2 inputLocation = {std::round(obj["x"].get<float>()*MAP_SCALE), std::round(obj["y"].get<float>()*MAP_SCALE)};
                if (obj.contains("properties")) {
                    for (auto &property : obj["properties"]) {
                        if (property["name"].get<std::string>() == "spawn index") {
                            spawnIndex = property["value"].get<int>();
                        }
                        if (property["name"].get<std::string>() == "spawn target") {
                            spawnToIndex = property["value"].get<int>();
                        }
                        if (property["name"] == "switchToMap") {
                            inputSwitchToMap = property["value"].get<std::string>();
                        }
                        if (property["name"] == "isAuto") {
                            isAuto = property["value"].get<bool>();
                        }
                        if (property["name"] == "direction") {
                            std::string temp_direction = property["value"].get<std::string>();
                            if (temp_direction == "top") {
                                direction = SwitchDirectionEnum::Top;
                            }
                            if (temp_direction == "bottom") {
                                direction = SwitchDirectionEnum::Bottom;
                            }
                            if (temp_direction == "left") {
                                direction = SwitchDirectionEnum::Left;
                            }
                            if (temp_direction == "right") {
                                direction = SwitchDirectionEnum::Right;
                            }
                        }
                    }
                }
                if (!isAuto) {
                    mapSwitchersList.emplace(spawnIndex, MapSwitcherProp(inputLocation, inputSwitchToMap, spawnIndex , spawnToIndex, width, height));
                } else {
                    autoMapSwitcherList.emplace(spawnIndex, AutoMapSwitcherProp(inputLocation, inputSwitchToMap, spawnIndex , spawnToIndex, width, height, direction));
                }
            }
        }
        auto shopLocationLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "shop location";
            });
        if (shopLocationLayer != layers.end()) {
            auto allShop = (*shopLocationLayer)["objects"];
            auto it = allShopItems.find(worldName);
            if (it != allShopItems.end()) {
                AllShopProperties worldShops = it->second;
                for (auto &shop: allShop) {
                    std::string tempShopName{};
                    if (shop.contains("properties")) {
                        for (auto &property: shop["properties"]) {
                            if (property["name"] == "shopName") {
                                tempShopName = property["value"];
                            }
                        }
                    }
                    Rectangle tempDimension = {shop["x"].get<float>()*MAP_SCALE, shop["y"].get<float>()*MAP_SCALE, shop["width"].get<float>()*MAP_SCALE, shop["height"].get<float>()*MAP_SCALE};
                    auto it_2 = worldShops.find(tempShopName);
                    if (it_2 != worldShops.end()) {
                        std::vector<ShopItemProperties> availableItemsProperties = it_2->second;
                        shops.emplace_back(tempDimension, tempShopName, availableItemsProperties);
                    }
                }
            }
        }
    }
}
std::vector<Car>* WorldSet::getCarList() {
    return &carList;
}
std::vector<InteractablePropV2>* WorldSet::getInteractableV2List() {
    return &interactableV2List;
}
std::vector<Drawing*> WorldSet::getMapLayers() {
    std::vector<Drawing*> layers{};
    for (MapLayer& layer: mapLayers) {
        layers.push_back(&layer);
    }
    return layers;
};
WorldEnums WorldSet::getWorldName() {
    return worldName;
}
std::vector<Shop>* WorldSet::getCurrentWorldShops() {
    return &shops;
};
std::vector<ShopItem>* WorldSet::getShopItems (std::string name) {
    auto shopToOpen = std::find_if(shops.begin(), shops.end(), [name](Shop shop){
        return shop.getShopName() == name;
    });
    std::vector<ShopItem>* shopItems = (*shopToOpen).getShopItems();
    return shopItems;
};
int WorldSet::getCollisionCode() {
    return collisionCode;
}
void WorldSet::changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height,
    Vector2 des, std::vector<int>* collisionData) {
        background.changeProperty( width, height, des, collisionData);
}

World::World(const char* inputTexture, WorldDrawProperty* drawProperty): worldTexture(LoadTexture(inputTexture)), drawProperty(drawProperty) {

}
void World::changeProperty (int width, int height, Vector2 des, std::vector<int>* collisionData) {
    drawProperty->changeProperty(width, height, des, collisionData);
}
std::vector<std::vector<int>>* WorldSet::getWorldCollisionArray () {
    return background.getWorldCollisionArray();
}
std::vector<std::vector<int>>* World::getWorldCollisionArray() {
    return &drawProperty->collisionArray;
}
void World::draw(Vector2 des) {
    DrawTextureEx(worldTexture, des, 0,MAP_SCALE,WHITE);
}
MapLayer::MapLayer(const char* imgSrc, int level): Drawing(0, level) , layerTexture(LoadTexture(imgSrc)) {

}
void MapLayer::draw(Vector2 mapPos) {
    DrawTextureEx(layerTexture, mapPos, 0,MAP_SCALE,WHITE);
}
std::vector<Drawing*> WorldSet::getAllDrawableProps () {
    std::vector<Drawing*> allWorldProps{};
    for (Drawing* propSet : *worldProps) {
        allWorldProps.push_back(propSet);
    }
    for (Drawing &item : interactableItemList) {
        allWorldProps.push_back(&item);
    }
    for (auto &[key, prop] : trafficLights) {
        Drawing* drawableProp = &prop;
        allWorldProps.push_back(drawableProp);
    }
    return allWorldProps;
};
void WorldSet::animateWorldProps(float deltaTime) {
    for (MapProp* propSet : *worldProps) {
        propSet->updateAnimation(deltaTime);
    }
    for (InteractablePropV2 &prop : interactableV2List) {
        prop.updateAnimation(deltaTime);
    }
    for (auto &[key, lightSet] : trafficLights) {
        lightSet.tick(deltaTime);
    }
    for (Car &car : carList) {
        bool isCarShouldMove {false};
        if (trafficLights.size() > 0) {
            for (auto &[key, lightSet] : trafficLights) {
                bool isCollide = checkIsCollide(car.getDimension(), lightSet.getCollision()).isCollide;
                if ((isCollide && lightSet.getLightState() == TrafficLightState::Red))
                {
                    isCarShouldMove = false;
                    break;
                } else {
                    isCarShouldMove = true;
                }
            }
            if (isCarShouldMove) {
                car.findDrivingPath(&carPathArray);
            } else {
                car.setDirection({0, 0});
            }
        } else {
            car.findDrivingPath(&carPathArray);
        }
        car.updateAnimation(deltaTime);
    }
}
Vector2 WorldSet::getSpawnLocation(int spawnIndex) {
    Vector2 spawnLocation{100, 100};
    auto it = mapSwitchersList.find(spawnIndex);
    if (it != mapSwitchersList.end()) {
        // key exists
        spawnLocation = Vector2Subtract(it->second.getSpawnLocation(), {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}) ;
        
    } else {
        auto it2 = autoMapSwitcherList.find(spawnIndex);
        if (it2 != autoMapSwitcherList.end()) {
            spawnLocation = Vector2Subtract(it2->second.getSpawnLocation(), {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}) ;
        } else {
            std::cout<<"can't find spawn point";
        }
    }
    return spawnLocation;
};
void WorldSet::saveAIPlayers(std::vector<AIPlayer> currentAIPlayers) {
    AIPlayers = currentAIPlayers;
};
std::unordered_map<int , LevelSwitcher>* WorldSet::getAutoLevelSwitcherList() {
    return &autoLevelSwitcherList;
};
std::unordered_map<int , LevelData>* WorldSet::getLevelDataList() {
    return &levelDataList;
};
void WorldSet::handleItemPickUp(Player& player, Vector2 mapPos) {
    if (player.getHoldingItems()->size() > 0) {
        interactableItemList.push_back((*player.getHoldingItems())[0]);
        player.getHoldingItems()->pop_back();
        return;
    }
    for (int i =0; i < interactableItemList.size() ; i++) {
        InteractableItem& item = interactableItemList[i];
        if (checkCircleInteraction(player.getCenter(mapPos), item.getCenter(mapPos), 100).isCollide) {
            player.replaceHoldingItems(item);
            interactableItemList.erase(interactableItemList.begin() + i);
            return;
        }
    }
}
std::vector<AIPlayer>* WorldSet::getAIPlayers() {
    return &AIPlayers;
}
std::vector<InteractableItem>* WorldSet::getInteractableItem() {
    return &interactableItemList;
};
MapSwitcherProp::MapSwitcherProp (Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height) : location(location), screenPos(location), switchToMap(switchToMap),
spawnIndex(spawnIndex), spawnToIndex(spawnToIndex), width(width), height(height) {
};
void MapSwitcherProp::draw(Vector2 des) {
    DrawRectangle(location.x + des.x, location.y + des.y, width, height, GREEN);
}
void MapSwitcherProp::setScreenPos (Vector2 mapPos) {
    screenPos = Vector2Add(location, mapPos);
}
Vector2 MapSwitcherProp::getSpawnLocation () {
    return location;
}
Rectangle MapSwitcherProp::getCollision() {
    Rectangle collisionBox = {screenPos.x, screenPos.y, (float)width, (float)height};
    return collisionBox;
}
AutoMapSwitcherProp::AutoMapSwitcherProp(Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height, SwitchDirectionEnum direction):
    MapSwitcherProp(location, switchToMap, spawnIndex, spawnToIndex, width, height),
    direction(direction) {
}
LevelSwitcher::LevelSwitcher(int option1, int option2, Rectangle dimension): option1(option1), option2(option2),
    dimension(dimension) {
}
Rectangle LevelSwitcher::getScreenPosDimension(Vector2 mapPos) {
    Rectangle screenPosDimension = {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height};
    return screenPosDimension;
}
void LevelSwitcher::draw(Vector2 mapPos) {
    Rectangle screenPosDimension = {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height};
    DrawRectangle(screenPosDimension.x, screenPosDimension.y, screenPosDimension.width, screenPosDimension.height, GREEN);
}
SwitchDirectionEnum AutoMapSwitcherProp::getDirection() {
    return direction;
}
void WorldSet::setSwitchersPos (Vector2 mapPos) {
    for (auto &[key, value] : mapSwitchersList) {
        value.setScreenPos(mapPos);
    }
    for (auto &[key, value] : autoMapSwitcherList) {
        value.setScreenPos(mapPos);
    }
}
void WorldSet::addItemtoWorld (InteractableItem item) {

};
std::unordered_map<int , MapSwitcherProp>* WorldSet::getMapSwitchersList(){
    return &mapSwitchersList;
};
std::unordered_map<int, AutoMapSwitcherProp>* WorldSet::getAutoMapSwitcherList() {
    return &autoMapSwitcherList;
};
SpawnToDetail MapSwitcherProp::getSwitchDestination() {
    SpawnToDetail spawnToDetail{};
    spawnToDetail.targetSpawnPoint = spawnToIndex;
    if (switchToMap == "office_gang_map") spawnToDetail.targetMap = CenterWorld;
    if (switchToMap == "office_interior") spawnToDetail.targetMap = InteriorOffice1;
    if (switchToMap == "supermarket_map") spawnToDetail.targetMap = InteriorSuperMarket;
    if (switchToMap == "office_interior_2") spawnToDetail.targetMap = InteriorOffice2;
    if (switchToMap == "hardware_interior") spawnToDetail.targetMap = InteriorHardware;
    if (switchToMap == "sunna_interior") spawnToDetail.targetMap = InteriorSunna;
    if (switchToMap == "upper_map") spawnToDetail.targetMap = WorldEnums::UpperMap;
    return spawnToDetail;
};