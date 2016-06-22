#pragma once
class Interface
{
private:
	int stars;
public:
	Interface();
	Interface(int);
	~Interface();

	void draw(void);
	void update(float deltaTime);
};

