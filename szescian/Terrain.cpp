#include "Terrain.h"
#include "Lazik.h"
#include "Wall.h"
#include <chrono>
#include <random.h>

int Terrain::potatoesAmount = Terrain::beginPotatoesAmount;

Terrain::Terrain() : wall(30, -10, 20 + 70, 5, 5)
{
    for (int i = 0; i < treesAmount; ++i) {
		Tree tree;
		trees.push_back(tree);
		switch (tree.sphereID) {
		case 1: 
			points1.push_back({tree.x, tree.z});
			break;
		case 2: 
			points2.push_back({tree.x, tree.z});
			break;
		case 3: 
			points3.push_back({tree.x, tree.z});
			break;
		case 4: 
			points4.push_back({tree.x, tree.z});
			break;
		}
    }
}

void Terrain::load() {
    plate = loadFile("plate.obj", "grass.jpg");
    mountains = loadFile("mountains.obj", "rock.jpg");
    for (int i = 0; i < treesAmount; i++) {
		trees[i].load();
    }
    loadPotatoes();
}

void Terrain::loadPotatoes()
{
    potatoes.clear();
    for (int i = 0; i < potatoesAmount; i++) {
        Potato potato;
        potato.object = loadFile("potato.obj", "potato.jpg");
        potato.x = random(minX + 100, maxX - 100);
        potato.z = random(minZ + 100, maxZ - 100);
        potato.angle = random(0, 360);
        potato.heightArgument = random(0, 200) / 100.0;
        potatoes.push_back(potato);
    }
}

void Terrain::draw()
{
    drawObj(&plate, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    drawObj(&mountains, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    for (int i = 0; i < treesAmount; i++) {
		trees[i].draw();
    }
    wall.draw();
}

void Terrain::checkPotatoes(POINT coords, int& points, ISoundEngine* soundEngine, int& level) {
    std::vector<int> toRm;
    int counter = 0;
    for (auto& potato : potatoes) {
        POINT point{ potato.x, potato.z };
        if (dist(point, coords) < collisionPointDistance + Lazik::collisionDistance) {
            points += 30;
            potatoCounter--;
            toRm.push_back(counter);
            soundEngine->play2D("audio/collect.mp3", false);
            if (potatoCounter == 0) {
                level++;
                Terrain::potatoesAmount += 3;
                loadPotatoes();
                potatoCounter = Terrain::potatoesAmount;
                soundEngine->play2D("audio/level.mp3", false);
            }
        }
        counter++;
    }
    for (auto i : toRm) {
        potatoes.erase(potatoes.begin() + i);
    }
}

bool Terrain::checkCollision(POINT coords) {
    int distance = Terrain::collisionPointDistance + Lazik::collisionDistance;
    if (coords.y < 0) {
        if (coords.x > 0) {
            for (auto point : points1) {
                if (dist(point, coords) < distance) {
                    return true;
                }
            }
        }
        else {
            for (auto point : points2) {
                if (dist(point, coords) < distance) {
                    return true;
                }
            }
        }
    }
    else {
        if (coords.x > 0) {
            for (auto point : points3) {
                if (dist(point, coords) < distance) {
                    return true;
                }
            }
        }
        else {
            for (auto point : points4) {
                if (dist(point, coords) < distance) {
                    return true;
                }
            }
        }
    }
    return false;
}

////

GLfloat Terrain::dist(POINT col, POINT laz) {
    return sqrt(pow((col.x - laz.x), 2) + pow((col.y - laz.y), 2));
}