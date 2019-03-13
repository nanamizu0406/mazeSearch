#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
#include<queue>
#include<chrono>
#include<thread>
#include<GL/glut.h>

const unsigned cellSize=20;
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
	int wightSearch();
	int heightSearch();
}search;

int main(int argc, char* argv[]){
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

	return 0;
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

