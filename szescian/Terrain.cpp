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
	sortWallsCollisions();
    fstream in;
    in.open("data.txt", ios::in);
    string a;
    in >> a;
    if (a != "") {
        personalBest = stoi(a);
    }
    in.close();
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
    pointsPot1.clear();
    pointsPot2.clear();
    pointsPot3.clear();
    pointsPot4.clear();
    for (int i = 0; i < potatoesAmount; i++) {
        Potato potato(Terrain::maxX, Terrain::maxZ, Terrain::minX, Terrain::minZ);
        switch (potato.sphereID) {
        case 1:
            pointsPot1.push_back(potato);
            break;
        case 2:
            pointsPot2.push_back(potato);
            break;
        case 3:
            pointsPot3.push_back(potato);
            break;
        case 4:
            pointsPot4.push_back(potato);
            break;
        }
    }
}

void Terrain::draw()
{
    glPushMatrix();
    glPolygonMode(GL_BACK, GL_LINE);
    drawObj(&plate, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    drawObj(&mountains, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    for (int i = 0; i < treesAmount; i++) {
		trees[i].draw();
    }
    wall.draw();
    glPopMatrix();
    for (auto& potato : pointsPot1) {
        glPushMatrix();
        glPolygonMode(GL_BACK, GL_LINE);
        glTranslatef(potato.x, 2 * sin(potato.heightArgument), potato.z);
        glRotatef(potato.angle, 0, 1, 0);
        potato.heightArgument += 0.1;
        if (potato.heightArgument > 1000) potato.heightArgument = 0;
        potato.angle += 1;
        if (potato.angle > 360) potato.angle = 0;
        potato.draw();
        glPopMatrix();
    }
    for (auto& potato : pointsPot2) {
        glPushMatrix();
        glPolygonMode(GL_BACK, GL_LINE);
        glTranslatef(potato.x, 2 * sin(potato.heightArgument), potato.z);
        glRotatef(potato.angle, 0, 1, 0);
        potato.heightArgument += 0.1;
        if (potato.heightArgument > 1000) potato.heightArgument = 0;
        potato.angle += 1;
        if (potato.angle > 360) potato.angle = 0;
        potato.draw();
        glPopMatrix();
    }
    for (auto& potato : pointsPot3) {
        glPushMatrix();
        glPolygonMode(GL_BACK, GL_LINE);
        glTranslatef(potato.x, 2 * sin(potato.heightArgument), potato.z);
        glRotatef(potato.angle, 0, 1, 0);
        potato.heightArgument += 0.1;
        if (potato.heightArgument > 1000) potato.heightArgument = 0;
        potato.angle += 1;
        if (potato.angle > 360) potato.angle = 0;
        potato.draw();
        glPopMatrix();
    }
    for (auto& potato : pointsPot4) {
        glPushMatrix();
        glPolygonMode(GL_BACK, GL_LINE);
        glTranslatef(potato.x, 2 * sin(potato.heightArgument), potato.z);
        glRotatef(potato.angle, 0, 1, 0);
        potato.heightArgument += 0.1;
        if (potato.heightArgument > 1000) potato.heightArgument = 0;
        potato.angle += 1;
        if (potato.angle > 360) potato.angle = 0;
        potato.draw();
        glPopMatrix();
    }
}

void Terrain::checkPotatoes(POINT coords, int& points, ISoundEngine* soundEngine, int& level) {
    std::vector<Potato>* pointsToCheck;
    if (coords.y < 0) {
        pointsToCheck = (coords.x > 0) ? &pointsPot1 : &pointsPot2;
    }
    else {
        pointsToCheck = (coords.x > 0) ? &pointsPot3 : &pointsPot4;
    }
    std::vector<int> toRm;
    int counter = 0;
    for (auto& pointPot : *pointsToCheck) {
        POINT point{ pointPot.x, pointPot.z };
        if (dist(point, coords) < collisionPointDistance + Lazik::collisionDistance) {
            points += 30;
            potatoesCounter--;
            toRm.push_back(counter);
            soundEngine->play2D("audio/collect.mp3", false);
            if (toRm.size() == 1 && potatoesCounter == 1) {
                level++;
                Terrain::potatoesAmount += 3;
                potatoesCounter = Terrain::potatoesAmount;
                loadPotatoes();
                soundEngine->play2D("audio/level.mp3", false);
                if (points > personalBest) {
                    personalBest = points;
                    fstream out;
                    out.open("data.txt", ios::out);
                    out << personalBest;
                    out.close();
                }
            }
        }
        counter++;
    }
    for (auto i : toRm) {
        pointsToCheck->erase(pointsToCheck->begin() + i);
    }
}

bool Terrain::checkCollision(POINT coords) {
    int distance = Terrain::collisionPointDistance + Lazik::collisionDistance;
    std::vector<POINT>* pointsToCheck;
    if (coords.y < 0) {
        pointsToCheck = (coords.x > 0) ? &points1 : &points2;
    }
    else {
        pointsToCheck = (coords.x > 0) ? &points3 : &points4;
    }
    for (auto point : *pointsToCheck) {
        if (dist(point, coords) < distance) {
            return true;
        }
    }
    return false;
}

////

GLfloat Terrain::dist(POINT col, POINT laz) {
    return sqrt(pow((col.x - laz.x), 2) + pow((col.y - laz.y), 2));
}

void Terrain::sortWallsCollisions()
{
    auto addPoint = [&](const POINT& p) {
        if (p.y < 0) {
            if (p.x > 0) {
                points1.push_back(p);
            }
            else {
                points2.push_back(p);
            }
        }
        else {
            if (p.x > 0) {
                points3.push_back(p);
            }
            else {
                points4.push_back(p);
            }
        }
    };
    // Wall
    for (int i = 0; i <= 125; i += 25) {
        addPoint({ i, 95 });
    }
    // Walls
    for (int i = minX; i < maxX; i++) {
        addPoint({ i, maxZ });
    }
    for (int i = minZ; i < maxZ; i++) {
        addPoint({ i, minX });
    }
    for (int i = minX; i < maxX; i++) {
        addPoint({ maxZ, i });
    }
    for (int i = minZ; i < maxZ; i++) {
        addPoint({ minX, i });
    }
}