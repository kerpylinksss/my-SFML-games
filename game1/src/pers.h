#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>


using namespace std;
using namespace sf;





class Entity {
protected:
	const float gravity = 0.0048f;
	float x, y, speed;
	float startJumpSpeed;
	float dx, dy;
	float groundLevel; // temporary, while i write a jump, then i will write a map and interaction with it:0
	int w, h;
	int varJump, currentJumpFrame, stepJumpAnim;
	int wStay, hStay;
	float health, stamine;
	bool life, isMove, onGround;
	enum stateOfPers{stay, left, right, jump, dead};
	enum dirOfJump { up, down, noDir};
	enum stateOfAtack{usualAtack, no};
	enum lastStateOfUsualAtack{lUsualAtack, rUsualAtack, noUsualAtack};
	enum lastStateOfPers{lleft, lright};
	stateOfPers state = stay;
	stateOfAtack atackState = no;
	dirOfJump dir = noDir;
	lastStateOfPers lstate;
	lastStateOfUsualAtack lAtackState = noUsualAtack;
	string File, FileRev;
	Texture texturePers, texturePersRev;
	Sprite spritePers;
	int xStay, xStayRev, yStay;
	map<int, vector<int>> mpFramesMove;
	map<int, vector<int>> mpFramesStay;
	int CurrentFrame, CurrentFrameRev;
	int CurrentFrameStay;
	//reversed frames:
	map<int, vector<int>> mpFramesMoveRev;
	map<int, vector<int>> mpFramesStayRev;
	//iterators for maps
	map<int, vector<int>>::const_iterator iterMove;
	map<int, vector<int>>::const_iterator iterMoveRev;
	map<int, vector<int>>::const_iterator iterStay;
	map<int, vector<int>>::const_iterator iterStayRev;

	//i will save frames in vector<IntRect> from this moment
	vector<IntRect> jumpFrames;
	vector<IntRect> jumpFramesRev;


	vector<IntRect> atackFrames;
	vector<IntRect> atackFramesRev;

	vector<IntRect> dieFrames;
	vector<IntRect> dieFramesRev;

	vector<IntRect>::iterator animIterator;
	virtual void forwardIteration() = 0;
	virtual void backIteration() = 0;

	virtual void move() = 0;
	virtual void Spawn(int x, int y, int startX, int startY);
	Clock clAnim, clJump;

	bool stated = false;
	bool statedRev = false;
public:
	Entity(float x, float y, float speed, int w, int h, float health, float stamine, string File, string FileRev);
	virtual void Update(const float &time, fstream &fs) = 0;
	void setFrames(int moveMain, const vector<int>& varMoveFrames, const vector<IntRect>& jumpFrames, const vector<IntRect>& atackFrames, const vector<IntRect>& dieFrames);
	void setFramesRev(int moveMain, vector<int>& varMoveFrames, const vector<IntRect>& jumpFramesRev, const vector<IntRect>& atackFramesRev, const vector<IntRect>& dieFramesRev);
	void setStayFrames(vector<int> stayFrames, vector<int> stayFramesRev, int stayY);
	Sprite getSprite();
	void setPersParameters(int wStay, int hStay);
};


// Player: 

class Player : public Entity {
private:
	enum stateOfSuperPunch{typeOne, typeTwo, noSuperPunch};
	stateOfSuperPunch superPunch;
	void move() override;
	void getJumpHeight(fstream& fs);
	bool alreadyTaken = false;
	void forwardIteration() override;
	void backIteration() override;
public:
	Player(float x, float y, float speed, int w, int h, float health, float stamine, string File, string FileRev, int startX, int startY, float groundLevel);
	void Update(const float &time, fstream &fs) override;
};




