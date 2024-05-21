#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include <iostream>

constexpr int windowWidth = 720;  // Width for the perspective view
constexpr int windowHeight = 480; // Height for the perspective view
constexpr int tileWidth = 14;     // Tile dimensions
constexpr int tileHeight = 14;

enum Direction {
    None,
    Left,
    Right,
    Up,
    Down
};

std::vector<std::vector<int>> map = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

Direction getDirectionKeyPressed() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        return Left;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        return Right;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        return Up;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        return Down;
    return None;
}

int main() {
    // Setup the main window for the first-person perspective
    sf::RenderWindow perspectiveWindow(sf::VideoMode(windowWidth, windowHeight), "First-Person View Smurf Engine");
    perspectiveWindow.setFramerateLimit(60);
    perspectiveWindow.setVerticalSyncEnabled(true);

    // Setup a separate window for the map view
    sf::RenderWindow mapWindow(sf::VideoMode(map.size() * tileWidth, map.size() * tileHeight), "Map View");
    mapWindow.setFramerateLimit(60);

    float playerPosX = 12.5f;
    float playerPosY = 12.5f;  //player start position

    float playerDirX = -1.0f;
    float playerDirY = 0.0f; //direction vector

    float cameraPlaneX = 0.0f;
    float cameraPlaneY = 0.66f; //camera plane

    // Setup for map and player
    sf::RectangleShape tile(sf::Vector2f(tileWidth - 1, tileHeight - 1));
    tile.setOutlineThickness(1);
    tile.setOutlineColor(sf::Color::Black);

    sf::CircleShape playerIndicator(5);
    playerIndicator.setFillColor(sf::Color::Red);
    playerIndicator.setOrigin(playerIndicator.getRadius(), playerIndicator.getRadius()); // Center Origin pf the circle

    sf::Clock clck = sf::Clock(); //for syncing movement
    sf::Time fps;
    while (perspectiveWindow.isOpen() && mapWindow.isOpen()) {
        sf::Event event;
        while (perspectiveWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                perspectiveWindow.close();
        }
        while (mapWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                mapWindow.close();
        }
        // Clear both windows
        perspectiveWindow.clear();
        mapWindow.clear();

        // Draw the map on the map window
        for (int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map.size(); x++) {
                tile.setPosition(x * tileWidth, y * tileHeight);
                tile.setFillColor(map[y][x] == 1 ? sf::Color::Blue : sf::Color::White);
                mapWindow.draw(tile);
            }
        }

        // Draw the player on the map
        mapWindow.draw(playerIndicator);

        // Perspective rendering here (in perspectiveWindow)
        for (int x = 0; x < windowWidth; x++) {
            //calculate ray-casting
            float cameraSpaceX = 2 * x / float(windowWidth) - 1; //x-coordinate in camera space
            float rayPosX = playerPosX;
            float rayPosY = playerPosY;
            float rayDirX = playerDirX + cameraPlaneX * cameraSpaceX;
            float rayDirY = playerDirY + cameraPlaneY * cameraSpaceX;

            //relative position
            int mapPosX = playerPosX;
            int mapPosY = playerPosY;

            //distance from current position to next wall
            float sideDistanceX;
            float sideDistanceY;

            //distance from one x/y-side to next x/y-side
            float deltaX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            float deltaY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

            //step direction (+1/-1)
            int sX=1;
            int sY=1;

            //calculate sideDistanceX/Y
            if (rayDirX < 0) {
                sX = -1;
                sideDistanceX = (rayPosX + (mapPosX * sX)) * deltaX;
            } else {
                sideDistanceX = (mapPosX + sX - rayPosX) * deltaX;
            }
            if (rayDirY < 0) {
                sY = -1;
                sideDistanceY = (rayPosY + (mapPosY * sY)) * deltaY;
            } else {
                sideDistanceY = (mapPosY + sY - rayPosY) * deltaY;
            }

            bool hit = false; // wall hit?
            bool side; // x/y-Side wall hit?
            // DDA
            while (!hit) {
                if (sideDistanceX < sideDistanceY) {
                    sideDistanceX += deltaX; //move in y direction
                    mapPosX += sX;
                    side = false;
                } else {
                    sideDistanceY += deltaY;
                    mapPosY += sY; //move in x direction
                    side = true;
                }
                //Collision check
                if (map[mapPosX][mapPosY] > 0) hit = true;
            }

            //wall color
            sf::Color wallColor = sf::Color::Blue;
            //simple shading
            if (side) {
                wallColor = sf::Color(wallColor.r / 1.5f, wallColor.g / 1.5f, wallColor.b / 1.5f);
            }

            float perpDist;

            if (!side)
                perpDist = fabs((mapPosX - rayPosX + (1 - sX) / 2) / rayDirX);  // distance projected in camera direction
            else
                perpDist = fabs((mapPosY - rayPosY + (1 - sY) / 2) / rayDirY);

            int lHeight = (int)abs((windowHeight / perpDist));

            //calculate lowest and highest pixel in stripe
            int renderStart = -lHeight / 2 + windowHeight / 2;
            int renderEnd = lHeight / 2 + windowHeight / 2;

            if (renderStart < 0)renderStart = 0;
            if (renderEnd >= windowHeight)renderEnd = windowHeight - 1;


            //draw pixels of the stripe as vertical line
            sf::Vertex lines[2] =
                    {
                            sf::Vertex(sf::Vector2f(x, renderStart), wallColor),
                            sf::Vertex(sf::Vector2f(x, renderEnd), wallColor)
                    };
            perspectiveWindow.draw(lines, 2, sf::Lines);
        }

        playerIndicator.setPosition(playerPosY * tileWidth -1 , playerPosX * tileHeight + 1);


        fps = clck.getElapsedTime();
        clck.restart();
        std::cout << "FPS: " << 1/fps.asSeconds() << "." << std::endl;

        float moveSpeed = fps.asSeconds() * 4.0f; //squares/second
        float rotSpeed = fps.asSeconds() * 2.0f; //radians/second

        Direction key = getDirectionKeyPressed();
        switch (key) {
            case Left: {
                float oldPlayerDirX = playerDirX;
                playerDirX = playerDirX * cos(rotSpeed) - playerDirY * sin(rotSpeed); // rotate perspective with rotationmatrix
                playerDirY = oldPlayerDirX * sin(rotSpeed) + playerDirY * cos(rotSpeed);
                float oldCameraPlaneX = cameraPlaneX;
                cameraPlaneX = cameraPlaneX * cos(rotSpeed) - cameraPlaneY * sin(rotSpeed);
                cameraPlaneY = oldCameraPlaneX * sin(rotSpeed) + cameraPlaneY * cos(rotSpeed);
                break;
            }
            case Right: {
                float oldPlayerDirX = playerDirX;
                playerDirX = playerDirX * cos(-rotSpeed) - playerDirY * sin(-rotSpeed);
                playerDirY = oldPlayerDirX * sin(-rotSpeed) + playerDirY * cos(-rotSpeed);
                float oldCameraPlaneX = cameraPlaneX;
                cameraPlaneX = cameraPlaneX * cos(-rotSpeed) - cameraPlaneY * sin(-rotSpeed);
                cameraPlaneY = oldCameraPlaneX * sin(-rotSpeed) + cameraPlaneY * cos(-rotSpeed);
                break;
            }
            case Up: {
                if (map[int(playerPosX + playerDirX * moveSpeed)][int(playerPosY)] == false)
                    playerPosX += playerDirX * moveSpeed;
                if (map[int(playerPosX)][int(playerPosY + playerDirY * moveSpeed)] == false)
                    playerPosY += playerDirY * moveSpeed;
                break;
            }
            case Down: {
                if (map[int(playerPosX - playerDirX * moveSpeed)][int(playerPosY)] == false)
                    playerPosX -= playerDirX * moveSpeed;
                if (map[int(playerPosX)][int(playerPosY - playerDirY * moveSpeed)] == false)
                    playerPosY -= playerDirY * moveSpeed;
                break;
            }
            case None:
            default:
                // Handle no key or any other case
                break;
        }

        // Draw Frames for both windows
        perspectiveWindow.display();
        mapWindow.display();
        perspectiveWindow.clear();
        mapWindow.clear();
    }

    return EXIT_SUCCESS;
}
