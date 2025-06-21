#include <SFML/Graphics.hpp>
#include<cstdlib>
#include<sstream>
#include<SFML/Audio.hpp>



using namespace sf;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side { LEFT, RIGHT, NONE};
side branchPosition[NUM_BRANCHES];


int main(){
	VideoMode vm(960,540);
	RenderWindow window(vm,"Timber!!!!");
	
	View view(FloatRect(0,0,1920,1080));
	window.setView(view);
	
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0,0);
	
	//make a tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810,0);
	
	//bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0,800);
	//is the bee is currently moving
	bool beeActive=false;
	//how fast can bee fly
	float beeSpeed=0.0f;
	
	//make 3 cloud sprite from 1 texture
	Texture textureCloud;
	
	//load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");
	//3 new sprite 1 texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	
	//position of cloud on the left of screen
	spriteCloud1.setPosition(0,0);
	spriteCloud2.setPosition(0,150);
	spriteCloud3.setPosition(0,300);
	
	//are the cloud currently on screen
	bool cloud1Active = false;
	bool cloud2Active =false;
	bool cloud3Active= false;
	
	//how fast is each cloud
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	
	//clock object
	Clock clock;
	//time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920/2) - timeBarStartWidth/2, 980);
	
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth/timeRemaining ;
	
	// Track whether the game is running
	bool paused = true;           
	// Draw some text
	int score = 0;
	
	Text messageText;
	Text scoreText;
	// We need to choose a font         
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
        // Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	// Assign the actual message
	messageText.setString("Press Enter To Start");
	scoreText.setString("Score=0");
	//font size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	//color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	//position
	FloatRect textRect = messageText.getLocalBounds();
	
	messageText.setOrigin(textRect.left + textRect.width/2.0f , textRect.top + textRect.height/2.0f);
	messageText.setPosition(1920/2.0f , 1080/2.0f);
	scoreText.setPosition(20,20);
	
	//PREPARE 6 BRANCHES
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for(int i=0; i<NUM_BRANCHES; i++){
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, - 2000);
		branches[i].setOrigin(220,20);
	}
	/*updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	*/
	//player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side  playerSide = side::LEFT;
	
	//rip
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);
	
	//axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;
	
	//log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
	
	bool acceptInput = false;
	
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	chop.setBuffer(deathBuffer);
	
	SoundBuffer ootBuffer;
	chopBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	

	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			if(event.type == Event::KeyPressed && !paused){
				acceptInput = true;
				spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
			}
		}
		
		Event event1;
		while(window.pollEvent(event1)){
			if(event1.type==event1.Closed){
				window.close();
			}	
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Escape)){
			window.close();
		}
		// Start the game
		if(Keyboard::isKeyPressed(Keyboard::Return)){
			paused = false;
			score = 0;
			timeRemaining =6;
			
			// Make all the branches disappear -
			// starting in the second position
			for(int i = 1; i<NUM_BRANCHES;i++){
				branchPosition[i] = side::NONE;	
			}
			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);
			// Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
			
			// Wrap the player controls to
			// Make sure we are accepting input
			// First handle pressing the right cursor key
			
		}
		
		if(acceptInput){
			if(Keyboard::isKeyPressed(Keyboard::Right)){
				playerSide=side::RIGHT;
				score++;
				// Add to the amount of time remaining
				timeRemaining += (2/score)+ .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				updateBranches(score);
					
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				chop.play();
					
					
			}
			if(Keyboard::isKeyPressed(Keyboard::Left)){
				playerSide=side::LEFT;
				score++;
				// Add to the amount of time remaining
				timeRemaining += (2/score)+ .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				updateBranches(score);
					
				// set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;
					
			}			
		}
		
		if(!paused){
		Time dt = clock.restart();
		
		// Subtract from the amount of time remaining
		timeRemaining-=dt.asSeconds();
		// size up the time bar
		timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
		
		if(timeRemaining <= 0.0f){
			paused = true;
			messageText.setString("OUT OF TIME!!");
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
			messageText.setPosition(1920/2.0f, 1080/2.0f);
			outOfTime.play();	
		}
		
		
		//setup the bee
		if(!beeActive){
			//how fast bee is
			srand((int)time(0)*10);
			beeSpeed = (rand() % 200) + 200;  //speed is between 200-400
			//how high bee is in
			srand((int)time(0)*10);
			float height = (rand() %500) + 500;//between 500-1000
			spriteBee.setPosition(2000, height);
			beeActive = true;
			
		}
		//to move the bee
		else{
			spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed*dt.asSeconds()), spriteBee.getPosition().y);
			//// Has the bee reached the right hand edge of the screen?
			if(spriteBee.getPosition().x<-100){
				beeActive = false;
				
			}
		}
		
		//blowing the cloud 1
		if(!cloud1Active){
			srand((int)time(0)*10);
			cloud1Speed = (rand() % 200);
			
			srand((int)time(0)*10);
			float height = (rand()%150);
			spriteCloud1.setPosition(-200, height); //it will move from left to right 
			cloud1Active = true;
					
		}
		else{
			spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed*dt.asSeconds()), spriteCloud1.getPosition().y);
			if(spriteCloud1.getPosition().x>1920){
				cloud1Active = false;
			}
		}
		
		//cloud 2 moving:::
		if(!cloud2Active){
			srand((int)time(0)*20);
			cloud2Speed = (rand() % 200);
			srand((int)time(0)*20);
			float height = (rand()%150) - 150;
			spriteCloud2.setPosition(-200,height);
			cloud2Active=true;
		}
		else{
			spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed*dt.asSeconds()), spriteCloud2.getPosition().y);
			if(spriteCloud2.getPosition().x>1920){
				cloud2Active = false;
			}
		
		}
		
		
		//cloud 3 moving:::
		if(!cloud3Active){
			srand((int)time(0)*30);
			cloud3Speed = (rand() % 200);
			srand((int)time(0)*30);
			float height = (rand()%450) - 150;
			spriteCloud3.setPosition(-200,height);
			cloud3Active=true;
		}
		else{
			spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed*dt.asSeconds()), spriteCloud3.getPosition().y);
			if(spriteCloud3.getPosition().x>1920){
				cloud3Active = false;
			}
		
		}
		// Update the score text
		std::stringstream ss;
		ss<< "Score "<<score;
		scoreText.setString(ss.str());
		//score++;
		// update the branch sprites
		for(int i =0; i<NUM_BRANCHES; i++){
			float height = i*150;
			if(branchPosition[i] == side::LEFT){
				branches[i].setPosition(610,height);
				branches[i].setRotation(180);
				
			}
			else if(branchPosition[i] == side::RIGHT){
				branches[i].setPosition(1330,height);
				branches[i].setRotation(0);
			}
			else{
				branches[i].setPosition(3000,height);
			}
		}
		if(logActive){
			spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX*dt.asSeconds()),spriteLog.getPosition().y + (logSpeedY*dt.asSeconds()));
			
			if(spriteLog.getPosition().x<-100 || spriteLog.getPosition().x>2000){
				logActive = false;
				spriteLog.setPosition(810, 720);
			
			}
		}
		if(branchPosition[5] == playerSide){
			paused = true;
			acceptInput = false;
			spriteRIP.setPosition(525,760);
			spritePlayer.setPosition(2000,660);
			messageText.setString("SQISHED!!");
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
			messageText.setPosition(1920/2.0f, 1080/2.0f);
			spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			death.play();
					
		}
		
		
		}
		
		window.clear();
		window.draw(spriteBackground);
		
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for(int i=0;i<NUM_BRANCHES;i++){
			window.draw(branches[i]);
		}
		
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		
		if(paused){
			window.draw(messageText);
		}
		window.display();
	}
	return 0;
}
//function definition
void updateBranches(int seed){
	for(int j = NUM_BRANCHES-1; j>0; j--){
		branchPosition[j] = branchPosition[j-1];
		
	}
	srand((int)time(0)+seed);
	int r =(rand()%5);
	switch(r){
		case 0:
			branchPosition[0] = side::LEFT;
			break;
		case 1:
			branchPosition[0] = side::RIGHT;
			break;
		default:
			branchPosition[0] = side::NONE;
			break;
			

	}

}




