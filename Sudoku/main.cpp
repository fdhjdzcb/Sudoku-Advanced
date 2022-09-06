#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#pragma warning(disable : 4996)


using namespace std;
using namespace sf;
vector<vector<int>> back_mat;
vector<vector<int>> front_mat;
vector<vector<int>> tmp_mat;
vector<vector<int>> type_mat;

int Size, SizeSquared;
int SolveCounter = 0;

vector<char> alphabet_fun(int abcd) { //Функция выбора алфавита
	if (abcd == 1)
		return { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'R' };
	if (abcd == 2)
		return { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	if (abcd == 3)
		return { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'p', 'r' };
	if (abcd == 4)
		return { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
}

void transpon(vector<vector<int>>& a) { //Транспонирование матрицы
	for (int i = 0; i < SizeSquared; i++)
		for (int j = i; j < SizeSquared; j++)
			swap(a[i][j], a[j][i]);
}

void generation(vector<vector<int>>& back_mat) { //Генерация
	back_mat.resize(SizeSquared);
	for (int i = 0; i < SizeSquared; i++)
		back_mat[i].resize(SizeSquared);
	type_mat = back_mat;

	char filename[12];
	sprintf_s(filename, "media/%d.txt", Size);
	ifstream file(filename);
	for (int i = 0; i < SizeSquared; i++)
		for (int j = 0; j < SizeSquared; j++) {
			file >> back_mat[i][j];
			type_mat[i][j] = 1;
		}

	int chng, region;
	int col1, col2, row1, row2, reg1, reg2;
	for (int j = 0; j < 100000; j++) {
		chng = rand() % 5;
		if (chng == 0)
			transpon(back_mat);
		if (chng == 1) {
			region = rand() % Size;
			row1 = rand() % Size;
			do
				row2 = rand() % Size;
			while (row1 == row2);
			for (int i = 0; i < SizeSquared; i++)
				swap(back_mat[region * Size + row1][i], back_mat[region * Size + row2][i]);
		}
		if (chng == 2) {
			region = rand() % Size;
			col1 = rand() % Size;
			do
				col2 = rand() % Size;
			while (col1 == col2);
			for (int i = 0; i < SizeSquared; i++)
				swap(back_mat[i][region * Size + col1], back_mat[i][region * Size + col2]);
		}
		if (chng >= 3) {
			chng = rand() % 2;
			if (chng == 1) transpon(back_mat);
			reg1 = rand() % Size;
			do
				reg2 = rand() % Size;
			while (reg1 == reg2);
			for (int i = 0; i < Size; i++)
				swap(back_mat[reg1 * Size + i], back_mat[reg2 * Size + i]);
			if (chng == 1) transpon(back_mat);
		}
	}
	front_mat = back_mat;
}

void MakeOwn(vector<vector<int>>& back_mat, int alphabet)  //Задает начальные условия для собственной матрицы
{
	back_mat.resize(SizeSquared);
	for (int i = 0; i < SizeSquared; i++)
		back_mat[i].resize(SizeSquared);
	for (int i = 0; i < SizeSquared; i++)
		for (int j = 0; j < SizeSquared; j++)
			back_mat[i][j] = 0;
	front_mat = type_mat = back_mat;
}

void hider(vector<vector<int>>& front_mat, int difficulty) //Скрывает клетки в зависимости от сложности
{
	int i, j;
	for (int k = 0; k < ((difficulty - 4 * (Size / 4) - 10 * (Size / 5)) * SizeSquared * SizeSquared / 100); k++) {
		do {
			i = rand() % SizeSquared;
			j = rand() % SizeSquared;
		} while (front_mat[i][j] == 0);
		front_mat[i][j] = type_mat[i][j] = 0;
	}
}

bool isPresentInCol(int col, int num, vector<vector<int>>& a) {
	for (int row = 0; row < SizeSquared; row++)
		if (a[row][col] == num)
			return true;
	return false;
}
bool isPresentInRow(int row, int num, vector<vector<int>>& a) {
	for (int col = 0; col < SizeSquared; col++)
		if (a[row][col] == num)
			return true;
	return false;
}
bool isPresentInBox(int boxStartRow, int boxStartCol, int num, vector<vector<int>>& a) {
	for (int row = 0; row < Size; row++)
		for (int col = 0; col < Size; col++)
			if (a[row + boxStartRow][col + boxStartCol] == num)
				return true;
	return false;
}
bool findEmptyPlace(int& row, int& col, vector<vector<int>>& a) {
	for (row = 0; row < SizeSquared; row++)
		for (col = 0; col < SizeSquared; col++)
			if (a[row][col] == 0)
				return true;
	return false;
}
bool isValidPlace(int row, int col, int num, vector<vector<int>>& a) {
	return !isPresentInRow(row, num, a) && !isPresentInCol(col, num, a) && !isPresentInBox(row - row % Size, col - col % Size, num, a);
}

bool solveSudoku(vector<vector<int>>& a) {
	int row, col;
	if (!findEmptyPlace(row, col, a))
		return true;

	for (int num = 1; num <= SizeSquared; num++) {
		if (isValidPlace(row, col, num, a)) {
			a[row][col] = num;
			if (solveSudoku(a))
				return true;
			a[row][col] = 0;
		}
	}
	return false;
}

bool countSudoku(vector<vector<int>>& a) {
	int row, col;
	if (SolveCounter > 100)
		return (SolveCounter);
	if (!findEmptyPlace(row, col, a))
	{
		SolveCounter++;
		return true;
	}
	for (int num = 1; num <= SizeSquared; num++) {
		if (isValidPlace(row, col, num, a)) {
			a[row][col] = num;
			countSudoku(a);
			a[row][col] = 0;
		}
	}
	return (SolveCounter);
}

bool EmptyCells(vector<vector<int>>& a)
{
	for (int i = 0; i < SizeSquared; i++)
		if (find(a[i].begin(), a[i].end(), 0) != a[i].end())
			return true;
	return false;
}

int CountEmptyCells(vector<vector<int>>& a)
{
	int count = 0;
	for (int i = 0; i < SizeSquared; i++)
		for (int j = 0; j < SizeSquared; j++)
			if (a[i][j] == 0)
				count++;
	return count;
}

void hinter(vector<vector<int>>& a) {
	int empty = 0;
	for (int i = 0; i < SizeSquared; i++)
		for (int j = 0; j < SizeSquared; j++)
			if (front_mat[i][j] == 0)
				empty++;
	int cellnum = rand() % empty + 1;
	for (int i = 0; i < SizeSquared; i++)
		for (int j = 0; j < SizeSquared; j++) {
			if (front_mat[i][j] == 0)
				cellnum--;
			if (!cellnum) {
				front_mat[i][j] = back_mat[i][j];
				return;
			}
		}
}

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	int ShiftToCentre, alphabetnum = 0, difficulty = 0;
	vector<char> alphabet;

	RenderWindow window(VideoMode(1000, 1000), "Sudoku!", Style::Close | Style::Titlebar);
	int width = 32;
	int x_texture = 0;
	Texture CellTexture;
	CellTexture.loadFromFile("media/textures.png");
	Sprite cell(CellTexture);

	Texture Menu, bgtext;
	Menu.loadFromFile("media/Menu.png");
	bgtext.loadFromFile("media/background.png");
	Sprite start(Menu), stats(Menu), exit(Menu), star(Menu), blackstar(Menu), man(Menu), blackman(Menu), next(Menu), backward(Menu), startOwn(Menu), hint(Menu), hinton(Menu), background(bgtext);
	Sprite arrowdown(Menu), arrowup(Menu), house(Menu), again(Menu), check(Menu);
	start.setTextureRect(IntRect(0, 0, 350, 200));
	stats.setTextureRect(IntRect(0, 200, 350, 75));
	exit.setTextureRect(IntRect(0, 275, 350, 75));
	star.setTextureRect(IntRect(0, 350, 150, 150));
	blackstar.setTextureRect(IntRect(0, 700, 150, 150));
	next.setTextureRect(IntRect(0, 500, 350, 100));
	backward.setTextureRect(IntRect(0, 600, 350, 100));
	man.setTextureRect(IntRect(150, 350, 150, 150));
	blackman.setTextureRect(IntRect(150, 700, 150, 150));
	startOwn.setTextureRect(IntRect(0, 0, 350, 95));
	hint.setTextureRect(IntRect(0, 850, 100, 100));
	hinton.setTextureRect(IntRect(100, 850, 100, 100));
	house.setTextureRect(IntRect(0, 950, 80, 80));
	again.setTextureRect(IntRect(80, 950, 80, 80));
	check.setTextureRect(IntRect(160, 950, 80, 80));
	arrowdown.setTextureRect(IntRect(200, 850, 100, 100));
	arrowup.setTextureRect(IntRect(200, 850, 100, 100));
	arrowup.rotate(180);

	bool isMainMenu = 1, isSecondMenu = 0, isStats = 0;
	int menuNum = 0;
	start.setPosition(350, 300);
	stats.setPosition(350, 500);
	exit.setPosition(350, 600);
	man.setPosition(25, 300);
	blackman.setPosition(25, 300);
	startOwn.setScale(0.95, 0.95);
	startOwn.setPosition(350, 0);
	hint.setPosition(900, 0);
	hinton.setPosition(900, 0);
	house.setPosition(10, 10);
	again.setPosition(90, 10);
	check.setPosition(160, 10);
	arrowdown.setPosition(800, 550);
	arrowup.setPosition(900, 450);

	backward.setPosition(60, 850);
	next.setPosition(620, 850);

	Texture messages;
	messages.loadFromFile("media/messages.png");
	Sprite win(messages), tryagain(messages), NumOfSolutions(messages), NoSolutions(messages), FillMore(messages);
	win.setTextureRect(IntRect(0, 0, 300, 80)); win.setPosition(355, 0);
	tryagain.setTextureRect(IntRect(0, 80, 575, 60)); tryagain.setPosition(215, 30);
	NumOfSolutions.setTextureRect(IntRect(0, 140, 410, 50)); NumOfSolutions.setPosition(235, 25);
	NoSolutions.setTextureRect(IntRect(0, 190, 350, 50)); NoSolutions.setPosition(235, 25);
	FillMore.setTextureRect(IntRect(0, 240, 575, 40)); FillMore.setPosition(215, 30);

	background.setPosition(0, 0);

	Font font;
	font.loadFromFile("media/SourceCodePro-Regular.ttf");
	Text text("", font, 32 - 2 * (alphabetnum / 2));
	text.setColor(Color::Black);

	Font MenuFont;
	MenuFont.loadFromFile("media/MenuFont.ttf");
	Text MenuText("", MenuFont, 70); MenuText.setColor(Color::Black);
	Text SolveNumWindow("", MenuFont, 50); SolveNumWindow.setColor(Color::Black);
	SolveNumWindow.setPosition(655, 15);
	Text HintNumWindow("", MenuFont, 30); HintNumWindow.setColor(Color::Black);
	HintNumWindow.setPosition(960, 70);

	Music music;//создаем объект музыки
	music.openFromFile("media/music.ogg");//загружаем файл
	music.play();//воспроизводим музыку
	music.setLoop(true);
	music.setVolume(15);

	vector<Text> GameSets(9);
	int MenuCount = 0;
	for (auto& text : GameSets) {
		text.setFont(MenuFont);
		text.setCharacterSize(70);
	}
	GameSets[8].setFillColor(Color::Black);
	GameSets[8].setPosition(340, 915);

	int text_num, i_chosen = -1, j_chosen = -1, tmp;
	char input; bool RightStarBlack, MButtonReleased = false;
	int timer = 0, sec = 0, min = 0, hour = 0; char time[9];
	int HintNum; bool Fails;
	int TimeMessage = 0, NumOfMessage = 0;

	ifstream statistics;
	vector<vector<Text>> StatsStorage;
	int LineNum, PageNum = 0; bool StatsGot = 0;

	ofstream fileo;

	bool GameWon = false;

	while (window.isOpen())
	{
		window.setFramerateLimit(60);
		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		if (isMainMenu) {  //Главное меню
			start.setColor(Color::White);
			stats.setColor(Color::White);
			exit.setColor(Color::White);
			menuNum = 0;

			if (IntRect(350, 300, 400, 200).contains(Mouse::getPosition(window))) { start.setColor(Color(255, 255, 255, 128)); menuNum = 1; }
			if (IntRect(350, 500, 300, 75).contains(Mouse::getPosition(window))) { stats.setColor(Color(255, 255, 255, 128)); menuNum = 2; }
			if (IntRect(350, 600, 300, 75).contains(Mouse::getPosition(window))) { exit.setColor(Color(255, 255, 255, 128)); menuNum = 3; }

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menuNum == 1) {
					isMainMenu = false; isSecondMenu = true;
				}
				if (menuNum == 2) {
					isMainMenu = false;
					isStats = true;
				}
				if (menuNum == 3) { window.close(); isMainMenu = false; }
			}

			window.draw(background);
			window.draw(start);
			window.draw(stats);
			window.draw(exit);
		}
		else if (isSecondMenu) {  //Второе меню
			window.clear(Color::White);
			next.setColor(Color::White);
			backward.setColor(Color::White);
			char size_button[6];
			if (IntRect(30, 850, 350, 100).contains(Mouse::getPosition(window))) { backward.setColor(Color(255, 255, 255, 128)); if (Mouse::isButtonPressed(Mouse::Left)) { isMainMenu = 1; isSecondMenu = 0; } }  //кнопка назад
			if (IntRect(650, 850, 350, 100).contains(Mouse::getPosition(window))) {  //кнопка вперед
				next.setColor(Color(255, 255, 255, 128));
				if (Mouse::isButtonPressed(Mouse::Left) and alphabetnum and Size)
				{
					isSecondMenu = 0;
					SizeSquared = Size * Size;
					ShiftToCentre = 500 - SizeSquared * 16 - 5 * (Size - 1) / 2;
					alphabet = alphabet_fun(alphabetnum);
					if (difficulty) {
						generation(back_mat);
						hider(front_mat, 15 + difficulty * 7);
						HintNum = CountEmptyCells(front_mat) * 0.2;
						i_chosen = j_chosen = -1;
						GameWon = false;
						timer = sec = min = hour = 0;
					}
					else MakeOwn(back_mat, Size);
				}
			}
			for (int i = 2; i <= 5; i++) {
				sprintf_s(size_button, "%dx%d", i * i, i * i);
				GameSets[i - 2].setString(size_button);
				GameSets[i - 2].setPosition(-320 + i * 200 + (i / 5) * 50, 100);
				if (IntRect(-320 + i * 200 + (i / 5) * 50, 100, 200, 100).contains(Mouse::getPosition(window)) or i == Size) {
					GameSets[i - 2].setFillColor(Color(0, 0, 0, 128));
					if (Mouse::isButtonPressed(Mouse::Left))
						Size = i;
				}
				else GameSets[i - 2].setColor(Color::Black);
				window.draw(GameSets[i - 2]);
			}

			if (IntRect(25, 300, 150, 150).contains(Mouse::getPosition(window)) or !difficulty) {
				window.draw(blackman);
				if (Mouse::isButtonPressed(Mouse::Left))
					difficulty = 0;
			}
			else window.draw(man);

			RightStarBlack = false;
			for (int i = 5; i >= 1; i--) {
				star.setPosition(25 + i * 160, 300);
				blackstar.setPosition(25 + i * 160, 300);
				if (IntRect(25 + i * 160, 300, 160, 150).contains(Mouse::getPosition(window)) or difficulty >= i or RightStarBlack) {
					window.draw(blackstar);
					RightStarBlack = true;
					if (IntRect(25 + i * 160, 300, 160, 150).contains(Mouse::getPosition(window)) and Mouse::isButtonPressed(Mouse::Left))
						difficulty = i;
				}
				else window.draw(star);
			}

			GameSets[4].setString("1..9A..Z"); GameSets[5].setString("A..Z"); GameSets[6].setString("1..9a..z"); GameSets[7].setString("a..z");
			for (int i = 4; i <= 7; i++) {
				GameSets[i].setPosition(50 + 190 * (i - 4) + 100 * (i / 6 + i % 2), 600);
				if (IntRect(50 + 190 * (i - 4) + 100 * (i / 6 + i % 2), 600, 200, 150).contains(Mouse::getPosition(window)) or (i - 3 == alphabetnum)) {
					GameSets[i].setFillColor(Color(0, 0, 0, 128));
					if (Mouse::isButtonPressed(Mouse::Left))
						alphabetnum = i - 3;
				}
				else GameSets[i].setFillColor(Color::Black);
				window.draw(GameSets[i]);
			}

			window.draw(backward); window.draw(next);
		}
		else if (isStats) {
			window.clear(Color::White);
			if (!StatsGot) {
				PageNum = 0;
				statistics.open("media/statistics.txt");
				StatsStorage.clear();
				StatsStorage.shrink_to_fit();
				LineNum = 0;
				string s;
				int StatsX;

				while (!statistics.eof()) {
					StatsStorage.push_back({});
					StatsStorage[LineNum].resize(4);
					for (int i = 0; i < 3; i++) {
						statistics >> s;
						StatsStorage[LineNum][i + 1].setFont(MenuFont);
						StatsStorage[LineNum][i + 1].setCharacterSize(50);
						StatsStorage[LineNum][i + 1].setColor(Color::Black);
						StatsStorage[LineNum][i + 1].setString(s);
						switch (i)
						{
						case 0: StatsX = 200; break;
						case 1: StatsX = 400; break;
						case 2: StatsX = 500; break;
						}
						StatsStorage[LineNum][i + 1].setPosition(StatsX, 100 + (LineNum * 70) % 700);
					}
					LineNum++;
				}
				for (int i = 0; i < LineNum; i++) {
					StatsStorage[i][0].setFont(MenuFont);
					StatsStorage[i][0].setCharacterSize(50);
					StatsStorage[i][0].setColor(Color::Black);
					StatsStorage[i][0].setString(to_string(i + 1));
					StatsStorage[i][0].setPosition(100, 100 + (i * 70) % 700);
				}
				StatsGot = true;
				statistics.close();
			}
			else {
				int CurrLine;
				arrowdown.setPosition(800, 550);
				arrowup.setPosition(900, 450);
				CurrLine = 10;
				if (PageNum == LineNum / 10)
					CurrLine = StatsStorage.size() % 10;

				for (int i = 0; i < CurrLine; i++)
					for (int j = 0; j < 4; j++)
						window.draw(StatsStorage[i + PageNum * 10][j]);

				arrowdown.setColor(Color::Black);
				if ((StatsStorage.size() - 1) / 10 - PageNum) {
					if (IntRect(800, 550, 100, 100).contains(Mouse::getPosition(window))) {
						arrowdown.setColor(Color(255, 255, 255, 128));
						if (Mouse::isButtonPressed(Mouse::Left) and !MButtonReleased) {
							PageNum++;
							MButtonReleased = true;
						}
					}
					window.draw(arrowdown);
				}

				arrowup.setColor(Color::Black);
				if (PageNum > 0) {
					if (IntRect(800, 350, 100, 100).contains(Mouse::getPosition(window))) {
						arrowup.setColor(Color(255, 255, 255, 128));
						if (Mouse::isButtonPressed(Mouse::Left) and !MButtonReleased) {
							PageNum--;
							MButtonReleased = true;
						}
					}
					window.draw(arrowup);
				}
				if (!Mouse::isButtonPressed(Mouse::Left))
					MButtonReleased = false;
			}

			backward.setColor(Color::White);
			if (IntRect(30, 850, 350, 100).contains(Mouse::getPosition(window))) { backward.setColor(Color(255, 255, 255, 128)); if (Mouse::isButtonPressed(Mouse::Left)) { isMainMenu = 1; isStats = 0; StatsGot = 0; } }
			window.draw(backward);
		}
		else {
			window.clear(Color::White);

			if (!GameWon and difficulty) { //таймер
				timer++;
				if (timer == 60) {
					sprintf(time, "%02d:%02d:%02d", hour, min, sec);
					GameSets[8].setString(time);
					sec++;
					timer = 0;
					if (sec == 60) {
						min++;
						sec = 0;
						if (min == 60) {
							hour++;
							min = 0;
						}
					}
				}
				window.draw(GameSets[8]);
			}

			if (IntRect(900, 0, 100, 100).contains(Mouse::getPosition(window)) and difficulty and HintNum and EmptyCells(front_mat)) {
				hinton.setColor(Color::Black);
				window.draw(hinton);
				if (Mouse::isButtonPressed(Mouse::Left) and !MButtonReleased) {
					hinter(front_mat);
					HintNum--;
					MButtonReleased = true;
				}
				if (!Mouse::isButtonPressed(Mouse::Left))
					MButtonReleased = false;
				HintNumWindow.setString(to_string(HintNum));
				window.draw(HintNumWindow);
			}
			else {
				hint.setColor(Color(255, 255, 255, 128));
				window.draw(hint);
			}

			Fails = false;
			for (int i = 0; i < SizeSquared; i++)
				for (int j = 0; j < SizeSquared; j++)
				{
					if (IntRect(j * width + (j / Size) * 5 + ShiftToCentre, i * width + (i / Size) * 5 + ShiftToCentre, 32, 32).contains(Mouse::getPosition(window)) and type_mat[i][j] == 0 and !GameWon) text_num = 1;
					else text_num = 0;

					if ((Mouse::isButtonPressed(Mouse::Left) and !type_mat[i][j] and (text_num == 1)) or (j == j_chosen and i == i_chosen)) { //выбор клетки
						i_chosen = i;
						j_chosen = j;
						text_num = 2;
					}

					cell.setColor(Color::White);
					//if (i_chosen != -1 and (i == i_chosen or j == j_chosen or ((j / Size == j_chosen / Size) and (i / Size == i_chosen / Size))) and ((i != i_chosen) or (j != j_chosen)))
					if (i_chosen != -1 and (i == i_chosen and j == j_chosen))
						cell.setColor(Color(50, 50, 50, 32));
					if (i_chosen != -1 and front_mat[i][j] == front_mat[i_chosen][j_chosen] and front_mat[i_chosen][j_chosen] != 0)
						cell.setColor(Color(79, 185, 227, 127));

					tmp = front_mat[i][j]; //подсветка конфликтующих
					front_mat[i][j] = 0;
					if (!isValidPlace(i, j, tmp, front_mat) and tmp) {
						cell.setColor(Color(255, 0, 0, 127));
						Fails = true;
					}
					front_mat[i][j] = tmp;

					cell.setTextureRect(IntRect(32 * text_num, 0, width, width)); //вывод поля
					cell.setPosition(j * width + (j / Size) * 5 + ShiftToCentre, i * width + (i / Size) * 5 + ShiftToCentre);
					window.draw(cell);
					if (front_mat[i][j] != 0) {
						text.setString(alphabet[front_mat[i][j] - 1]);
						text.setPosition(j * width + (j / Size) * 5 + ShiftToCentre + 6 + (alphabetnum / 2), i * width + (i / Size) * 5 + ShiftToCentre - 5);
						window.draw(text);
					}
				}

			if (i_chosen != -1 and front_mat[i_chosen][j_chosen] != back_mat[i_chosen][j_chosen] and front_mat[i_chosen][j_chosen] and difficulty > 0) {  //что-то типа проверки после ввода
				if (cell.getColor() != Color(255, 0, 0, 127) and !Fails) {
					tmp_mat = front_mat;
					if (solveSudoku(tmp_mat))
						back_mat = tmp_mat;
				}
			}

			if (event.type == Event::TextEntered and (i_chosen != -1) and !GameWon)  //ввода символа
			{
				input = static_cast<char>(event.text.unicode);
				if ((alphabetnum == 1 or alphabetnum == 2) and (97 <= input and input <= 122)) //эта и следующие четыре за капс и !капс
					input -= 32;
				if ((alphabetnum == 3 or alphabetnum == 4) and (65 <= input and input <= 90))
					input += 32;
				if ((find(alphabet.begin(), alphabet.end(), input) != alphabet.end()) and (distance(alphabet.begin(), find(alphabet.begin(), alphabet.end(), input)) < SizeSquared)) //проверка на правильность ввода
					front_mat[i_chosen][j_chosen] = distance(alphabet.begin(), find(alphabet.begin(), alphabet.end(), input) + 1); //присваивание введенного
			}
			if ((event.key.code == Keyboard::Backspace or event.key.code == Keyboard::Delete) and i_chosen != -1 and !GameWon)  //стирание символа
				front_mat[i_chosen][j_chosen] = 0;

			if (IntRect(10, 10, 80, 80).contains(Mouse::getPosition(window))) { //кнопка домика, возвращает в главное меню
				house.setColor(Color::Black);
				if (Mouse::isButtonPressed(Mouse::Left)) {
					isMainMenu = true;
					TimeMessage = 0;
					hour = min = sec = timer = 0;
					sprintf(time, "%02d:%02d:%02d", hour, min, sec);
					GameSets[8].setString(time);
					i_chosen = j_chosen = -1;
				}
			}
			else house.setColor(Color(255, 255, 255, 128));
			window.draw(house);

			if (IntRect(90, 10, 80, 80).contains(Mouse::getPosition(window))) {  //кнопка рестарта
				again.setColor(Color::Black);
				if (Mouse::isButtonPressed(Mouse::Left)) {
					for (int i = 0; i < SizeSquared; i++)
						for (int j = 0; j < SizeSquared; j++)
							if (!type_mat[i][j])
								front_mat[i][j] = 0;
					HintNum = CountEmptyCells(front_mat) * 0.2;
					timer = sec = min = hour = 0;
					GameWon = false;
				}
			}
			else again.setColor(Color(255, 255, 255, 128));
			window.draw(again);

			if (IntRect(160, 10, 80, 80).contains(Mouse::getPosition(window))) { //кнопка проверки количества решений
				check.setColor(Color::Black);
				if (Size / 5 == 0 or difficulty > 0 or (CountEmptyCells(front_mat) < 0.36 * SizeSquared * SizeSquared)) {
					if (Mouse::isButtonPressed(Mouse::Left) and !MButtonReleased) {
						tmp_mat = front_mat;
						SolveCounter = 0;
						if (countSudoku(tmp_mat) and !Fails) { // and difficulty?
							NumOfMessage = 1; //сообщение "Нет решений"
						}
						else NumOfMessage = 2; //Сообщение "Решений всего: Х"
						TimeMessage = 150;
						MButtonReleased = true;
					}
					if (!Mouse::isButtonPressed(Mouse::Left))
						MButtonReleased = false;
				}
				else {
					FillMore.setPosition(215, 930);
					window.draw(FillMore);
				}
			}
			else check.setColor(Color(255, 255, 255, 128));
			window.draw(check);

			if (difficulty == 0 and EmptyCells(front_mat)) {  //создание собственного поля
				HintNum = 0;
				startOwn.setColor(Color::White);
				if (IntRect(380, 1, 350, 95).contains(Mouse::getPosition(window))) {
					startOwn.setColor(Color(255, 255, 255, 128));
					if (Size / 5 == 0 or (CountEmptyCells(front_mat) < 0.36 * SizeSquared * SizeSquared)) {
						if (Mouse::isButtonPressed(Mouse::Left) and EmptyCells(front_mat))
						{
							tmp_mat = front_mat;
							if (solveSudoku(tmp_mat) and !(Fails)) {
								back_mat = tmp_mat;
								difficulty = 6;
								for (int x = 0; x < SizeSquared; x++)
									for (int y = 0; y < SizeSquared; y++)
										if (front_mat[x][y])
											type_mat[x][y] = 1;
								HintNum = CountEmptyCells(front_mat) * 0.2;
							}
							else {
								NumOfMessage = 3;
								TimeMessage = 150;
							}
						}
					}
					else {
						FillMore.setPosition(215, 930);
						window.draw(FillMore);
					}
				}
				window.draw(startOwn);
			}

			if (!EmptyCells(front_mat)) {  //проверка на пустоту
				if (back_mat == front_mat) {  //проверка на правильность
					fileo.open("media/statistics.txt", ios_base::app);
					char SizeInStats[6];
					sprintf_s(SizeInStats, "%dx%d", SizeSquared, SizeSquared);
					if (!GameWon)
						fileo << '\n' << SizeInStats << ' ' << difficulty << ' ' << time;
					window.draw(win);
					GameWon = true;
					fileo.close();
				}
				else window.draw(tryagain);
			}
		}
		if (SolveCounter <= 100) SolveNumWindow.setString(to_string(SolveCounter));
		else SolveNumWindow.setString(">100");
		if (TimeMessage > 0 and !GameWon) {
			switch (NumOfMessage) {
			case 1: {
				window.draw(NumOfSolutions);
				window.draw(SolveNumWindow);
				break;
			}
			case 2:
				window.draw(NoSolutions);
				break;
			case 3:
				window.draw(tryagain);
				break;
			}
			TimeMessage--;
		}
		window.display();
	}
	return 0;
	system("pause");
}