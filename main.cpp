#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
#include<queue>
#include<chrono>
#include<thread>
#include<GL/glut.h>

const unsigned cellSize=20;

void inits();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display();

using point=std::pair<unsigned, unsigned>;

point operator+(const point &lhs, const point &rhs)
{
	point p = { lhs.first + rhs.first, lhs.second + rhs.second };
	return p;
}

bool operator==(const point &lhs, const point &rhs)
{
	return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

class Search{
private:
	enum state{
	ROAD=0,
	WALL=1
	};

	const std::vector<std::vector<unsigned>> field={
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,0,1,1,1,0,1,1,1,0,1,0,1},
		{1,0,1,0,0,0,1,0,0,0,1,0,1,0,1},
		{1,0,1,1,1,0,1,1,1,0,1,0,1,1,1},
		{1,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
		{1,0,1,0,1,1,1,1,1,1,1,1,1,0,1},
		{1,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
		{1,1,1,1,1,0,1,0,1,0,1,0,1,1,1},
		{1,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
		{1,0,1,0,1,0,1,1,1,0,1,1,1,1,1},
		{1,0,1,0,1,0,0,0,1,0,1,0,0,0,1},
		{1,0,1,0,1,1,1,0,1,0,1,0,1,0,1},
		{1,0,1,0,0,0,0,0,1,0,0,0,1,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	const std::vector<point> dir={
		{ -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 },
	};
	std::vector<std::vector<unsigned>> memo;
	point start;
	point goal;
private:
	void inits();
	unsigned wight() const;
	unsigned height() const;
	bool isOutField(const point obj) const;
public:
	Search();
	~Search();
	void printField() const;
	void printGlutField() const;
	void printGlutWall() const;
	int wightSearch();
	int heightSearch();
}search;

int main(int argc, char* argv[]){
	
	glutInit(&argc, argv);
	glutCreateWindow("search");
	inits();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	
	search.printField();

	double val;
	unsigned num=0;
	std::chrono::system_clock::time_point  start, end; 

	//幅優先探索
	start=std::chrono::system_clock::now();
	num=search.wightSearch();
	std::cout<<num<<"distance"<<std::endl;
	end=std::chrono::system_clock::now(); 
	val=std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
	std::cout<<val<<"micro"<<std::endl;

	//深さ優先探索
	start=std::chrono::system_clock::now();
	num=search.heightSearch();
	std::cout<<num<<"distance"<<std::endl;
	end=std::chrono::system_clock::now(); 
	val=std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
	std::cout<<val<<"micro"<<std::endl;

	glutMainLoop();
	
	return 0;
}

void inits(){
	static const unsigned WindowSizeWight=800;
	static const unsigned WindowSizeHeight=800;

	static const unsigned WindowPossitionWight=100;
	static const unsigned WindowPossitionHeight=100;

	gluOrtho2D(0, 100, 100, 0);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowPosition(WindowPossitionWight, WindowPossitionHeight);
	glutInitWindowSize(WindowSizeWight, WindowSizeHeight);

	glClearColor(0.0, 0.0, 0.0, 1.0);
}


void resize(int w, int h){
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w-0.5, (GLdouble)h-0.1, -0.5, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q':
	case 'Q':
	case '\033':
		std::exit(0);
		break;
	default:
		break;
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	search.printGlutField();
	search.printGlutWall();
	glFlush();
}

Search::Search(){
	this->inits();
	std::cout<<"start("<<this->start.first<<","<<this->start.second<<")"<<std::endl;
	std::cout<<"goal("<<this->goal.first<<","<<this->goal.second<<")"<<std::endl;
}

Search::~Search(){
	
}

void Search::inits(){
	
	this->start=std::make_pair(1, 1);
	this->goal=std::make_pair(this->wight()-2, this->height()-2);

	this->memo.clear();
	this->memo.resize(this->height());
	std::for_each(this->memo.begin(), this->memo.end(), [this](auto& vec){
			vec.resize(this->wight());
		});
	std::for_each(this->memo.begin(), this->memo.end(), [](auto& vec){
			std::fill(vec.begin(), vec.end(), 0);
		});	
}

unsigned Search::wight()const {
	return this->field.at(0).size();
}

unsigned Search::height() const{
	return this->field.size();
}

bool Search::isOutField(const point obj) const {
	return (obj.first>=0&&obj.first<this->wight())&&(obj.second>=0&&obj.second<this->height());
}

void Search::printField() const{
	std::for_each(this->field.begin(), this->field.end(), [](auto& vec){
			std::for_each(vec.begin(), vec.end(), [](auto& val){
					switch(val){
					case ROAD:
						std::cout<<"　";
						break;
					case WALL:
						std::cout<<"＊";
						break;
					default:
						break;
					}
				});
			std::cout<<std::endl;
		});
}

void Search::printGlutField() const{
	static const unsigned lineSize=1;
	
	glColor3f(0.0f, 1.0f, 0.0);
	glLineWidth(lineSize);
	glBegin(GL_LINES);
	
	for(int i=0;i<=this->wight();i++){
		glVertex2i(cellSize*i, 0);
		glVertex2i(cellSize*i, cellSize*this->wight());
	}
	for(int j=0;j<=this->height();j++){
		glVertex2i(0, cellSize*j);
		glVertex2i(cellSize*this->height(), cellSize*j);
	}
	
	glEnd();
}

void Search::printGlutWall() const{
	static const double val=cellSize/2;
	static const unsigned pointSize=19;
	
	glColor3f(0.0f, 0.0f, 0.8f);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for(int i=0;i<this->field.size();i++){
		for(int j=0;j<this->field.at(0).size();j++){
			if(this->field.at(i).at(j)==WALL)
				glVertex2d(val+cellSize*j, val+cellSize*i);
		}
	}
	glEnd();
}

int Search::wightSearch(){
	//幅優先探索
	static const double val=cellSize/2;
	static const unsigned pointSize=19;
	
	std::queue<point> q;
	
	q.push(this->start);
	while(!q.empty()){
		point cur=q.front();		
		q.pop();
		if(cur==this->goal)
			return this->memo[cur.first][cur.second];
		std::for_each(this->dir.begin(), this->dir.end(), [&, this](auto obj){
				point next=cur+obj;
				if(this->isOutField(next)){
					const unsigned state=this->field[next.first][next.second];
					if(state==ROAD){
						if(this->memo[next.first][next.second]==0){							
							q.push(next);
							this->memo[next.first][next.second]=this->memo[cur.first][cur.second]+1;
						}
					}
				}
			});
	}	
	return false;
}

int Search::heightSearch(){
	this->inits();
	//深さ優先探索
	std::stack<point> s;
	
	s.push(this->start);
	while(!s.empty()){
		point cur=s.top();		
		s.pop();
		if(cur==this->goal)
			return this->memo[cur.first][cur.second];
		std::for_each(this->dir.begin(), this->dir.end(), [&, this](auto obj){
				point next=cur+obj;
				if(this->isOutField(next)){
					const unsigned state=this->field[next.first][next.second];
					if(state==ROAD){
						if(this->memo[next.first][next.second]==0){
							s.push(next);
							this->memo[next.first][next.second]=this->memo[cur.first][cur.second]+1;
						}
					}
				}				
			});
	}	
	return false;	
}

