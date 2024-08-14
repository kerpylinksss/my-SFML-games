#include "pers.h"


Entity::Entity(float x, float y, float speed, int w, int h, float health, float stamine, string File, string FileRev) {
		this->speed = speed;
		this->startJumpSpeed = 2.f;
		this->w = w;
		this->h = h;
		this->health = health;
		this->stamine = stamine;
		this->File = File;
		this->FileRev = FileRev;
		this->texturePers.loadFromFile("../images/tilesEntity/" + File);
		this->texturePersRev.loadFromFile("../images/tilesEntityRev/" + FileRev);
		this->spritePers.setTexture(texturePers);
		this->spritePers.setOrigin(w/2, h/2);
	}

void Entity::setFrames(int moveMain, const vector<int>& varMoveFrames, const vector<IntRect> &jumpFrames, const vector<IntRect> &atackFrames, const vector<IntRect> &dieFrames) {
	this->mpFramesMove.insert(make_pair(moveMain, varMoveFrames));
	this->iterMove = mpFramesMove.cbegin();
	this->jumpFrames = jumpFrames;
	varJump = jumpFrames.size();
	stepJumpAnim = 1;
	currentJumpFrame = 0;
	this->atackFrames = atackFrames;
	this->dieFrames = dieFrames;
}

void Entity::setStayFrames(vector<int> stayFrames, vector<int> stayFramesRev, int stayY) {
	mpFramesStay.insert(make_pair(stayY, stayFrames));
	mpFramesStayRev.insert(make_pair(stayY, stayFramesRev));

	iterStay = mpFramesStay.cbegin();
	iterStayRev = mpFramesStayRev.cbegin();
}

void Entity::setFramesRev(int moveMain, vector<int>& varMoveFrames, const vector<IntRect> &jumpFramesRev, const vector<IntRect> &atackFramesRev, const vector<IntRect> &dieFramesRev){
	this->mpFramesMoveRev.insert(make_pair(moveMain, varMoveFrames));
	this->iterMoveRev = mpFramesMoveRev.cbegin();
	this->jumpFramesRev = jumpFramesRev;
	this->atackFramesRev = atackFramesRev;
	this->dieFramesRev = dieFramesRev;
}

void Entity::Spawn(int x, int y, int startPosX, int startPosY) {
	this->x = x;
	this->y = y;
	this->isMove = false;
	this->onGround = true;
	this->life = true;
	this->spritePers.setTextureRect(IntRect(startPosX, startPosY, w, h));
	this->spritePers.setPosition(Vector2f(x + w/2, y + h/2));
}

void Entity::setPersParameters(int wStay, int hStay) {
	this->wStay = wStay;
	this->hStay = hStay;
}

Sprite Entity::getSprite() {
	return this->spritePers;
}

// ...
// create class with bots enemies && class with Player
// create class with effects && link it to accidents
// create physics of move && add more parameters to the characters if needs such as
// mass;
// add different forces, maybe create a class with them
// ...



// Player: 


Player::Player(float x, float y, float speed, int w, int h, float health, float stamine, string File, string FileRev, int startX, int startY, float groundLevel): Entity(x, y, speed, w, h, health, stamine, File, FileRev) {
		superPunch = noSuperPunch;
		lstate = lright;
		state = stay;
		Spawn(x, y, startX, startY);
		this->groundLevel = y;
}

void Player::getJumpHeight(fstream &fs) {
	//string val = "Height of jump: " + to_string(groundLevel - y) + "\n";
	//fs << val << endl;
	cout << "Height of jump: " + to_string(groundLevel - y) + "\n";
}

void Player::move() {
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		dx = -speed;
		state = left;
		isMove = true;
		if (Keyboard::isKeyPressed(Keyboard::Space) && onGround) {
			state = jump;
			dir = up;
			onGround = false;
			isMove = true;
			dy = -startJumpSpeed;
			animIterator = jumpFrames.begin();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::D)) {
		dx = speed;
		state = right;
		isMove = true;
		if (Keyboard::isKeyPressed(Keyboard::Space) && onGround) {
			state = jump;
			dir = up;
			onGround = false;
			isMove = true;
			dy = -startJumpSpeed;
			animIterator = jumpFrames.begin();
		}
	}
	else if ((Keyboard::isKeyPressed(Keyboard::Space)) && (onGround)) {
		state = jump;
		dir = up;
		onGround = false;
		isMove = true;
		dy = -startJumpSpeed;
		animIterator = jumpFrames.begin();
	}
	else {
		state = stay;
	}
}

void Player::forwardIteration() {
	animIterator++;
}

void Player::backIteration() {
	animIterator--;
}

void Player::Update(const float &time, fstream &fs) {
	move();
	if (!onGround) {
		state = jump;
		dy += gravity * time;
		
		if (dy >= 0 && !alreadyTaken) {
			dir = down;
			getJumpHeight(fs);
			alreadyTaken = true;
		}
		if (y + dy * time >= groundLevel) {
			state = stay;
			stated = false;
			statedRev = false;
			isMove = false;
			onGround = true;
			alreadyTaken = false;
			dir = noDir;
			dy = 0;
			dx = 0;
			y = groundLevel;
		}
	}
	if (isMove) {
		switch (state) {
		case right:
			if (lstate != lright) {
				this->spritePers.setTexture(texturePers);
			}
			lstate = lright;
			if (clAnim.getElapsedTime() >= milliseconds(115)) {
				clAnim.restart();
				this->CurrentFrame++;
				if (CurrentFrame == (iterMove->second).size()) CurrentFrame = 0;
				this->spritePers.setTextureRect(IntRect(iterMove->second[CurrentFrame], iterMove->first, w, h));
			}
			break;
		case left:
			if (lstate != lleft) {
				this->spritePers.setTexture(texturePersRev);
			}
			lstate = lleft;
			if (clAnim.getElapsedTime() >= milliseconds(115)) {
				clAnim.restart();
				this->CurrentFrameRev--;
				if (CurrentFrameRev < 0) CurrentFrameRev = iterMoveRev->second.size() - 1;
				this->spritePers.setTextureRect(IntRect(iterMoveRev->second[CurrentFrameRev], iterMoveRev->first, w, h));
			}
			break;

		case jump:
			// animation of jump
			switch (lstate) {
			case lright:
				if (!stated) {
					stated = true;
					animIterator = jumpFrames.begin();
				}
				this->spritePers.setTexture(texturePers);
				switch (dir) {
				case up:
					if (clJump.getElapsedTime() >= milliseconds(150)) {
						clJump.restart();
						spritePers.setTextureRect(*animIterator);
						forwardIteration();
						if (animIterator == jumpFrames.end() - 1) {
							dir = down;
						}
					}
					break;
				case down:
					if (clJump.getElapsedTime() >= milliseconds(150)) {
						spritePers.setTextureRect(*animIterator);
						backIteration();
						if (animIterator == jumpFrames.begin()) {
							dir = noDir;
						}
					}
					break;
				default:
					break;
				}
				break;
			case lleft:
				if (!statedRev) {
					this->statedRev = true;
					animIterator = jumpFramesRev.begin();
				}
				this->spritePers.setTexture(texturePersRev);
				switch (dir) {
				case up:
					if (clJump.getElapsedTime() >= milliseconds(150)) {
						clJump.restart();
						spritePers.setTextureRect(*animIterator);
						forwardIteration();
						if (animIterator == jumpFramesRev.end() - 1) {
							dir = down;
						}
					}
					break;
				case down:
					if (clJump.getElapsedTime() >= milliseconds(150)) {
						spritePers.setTextureRect(*animIterator);
						backIteration();
						if (animIterator == jumpFramesRev.begin()) {
							dir = noDir;
						}
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		break;
		default:
			state = stay;
			isMove = false;
			dx = 0;
			dy = 0;
			break;
		}
	}
	else {
		if (state == stay) {
			switch (lstate) {
			case lleft:
				if (clAnim.getElapsedTime() >= milliseconds(110)) {
					clAnim.restart();
					this->CurrentFrameStay++;
					if (CurrentFrameStay == iterStayRev->second.size()) CurrentFrameStay = 0;
					this->spritePers.setTextureRect(IntRect(iterStayRev->second[CurrentFrameStay], iterStayRev->first, wStay, hStay));
				}
				break;
			case lright:
				if (clAnim.getElapsedTime() >= milliseconds(110)) {
					clAnim.restart();
					this->CurrentFrameStay++;
					if (CurrentFrameStay == iterStay->second.size()) CurrentFrameStay = 0;
					this->spritePers.setTextureRect(IntRect(iterStay->second[CurrentFrameStay], iterStay->first, wStay, hStay));
				}
				break;
			default:
				break;
			}
		}
	}

	x += dx * time;
	y += dy * time;
	spritePers.setPosition(Vector2f(x, y));
}
