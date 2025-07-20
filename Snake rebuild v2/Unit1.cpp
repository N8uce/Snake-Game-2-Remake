#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include <vcl.h>
#include <stdlib.h>
#include <vector>
#pragma comment(lib, "winmm.lib")
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
 __fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner){}
int x, y;//позиция змеи
int dx, dy;//направление движения
int len = 3, score = 3;// длина змеи и очки
int snake[1000][2];//массив змеи
bool gameover;// переменная
int apple_x, apple_y;// позиция яблока
int head_x, head_y; // позиция символа головы
int tail_dx , tail_dy; // направление хвоста
bool isPaused = false; //пауза
bool canChangeDirection = true; // Флаг, разрешающий изменение направления
int snake_dir[1000][2]; // массив направлений каждой части тела змеи
//функция которая генерит яблоко
void spawn_apple()
{
	srand(time(NULL));
	bool isOnSnake;
	do {
		isOnSnake = false;
		apple_x = rand() % Form1->StringGrid1->ColCount;
		apple_y = rand() % Form1->StringGrid1->RowCount;
		if (head_x == apple_x && head_y == apple_y) {
			isOnSnake = true;
		} else {
			for (int i = 0; i < len; i++) {
				if (snake[i][0] == apple_x && snake[i][1] == apple_y) {
					isOnSnake = true;
					break;
				}
			}
		}
	} while (isOnSnake);
	Form1->StringGrid1->Cells[apple_x][apple_y] = "A";
}
//функция описывающая начало игры
void new_game() {
	x = 5; y = 5; dx = 1; dy = 0; len = 3; score = 3;
	head_x = x; head_y = y; // Инициализация позиции головы
	for (int i = 0; i < len; i++) {
		snake[i][0] = x - i;
		snake[i][1] = y;
	}
	gameover = false;
	Form1->Timer1->Enabled = true;
	spawn_apple(); // Спавним первое яблоко
}
//функция описывающая конец игры
void game_over() {
	Form1->MainMenu1->Items->Items[0]->Visible = true;
	Form1->MainMenu1->Items->Items[1]->Visible = true;
	Form1->Image1->Visible = true;
	Form1->Timer1->Enabled = false;
	gameover = true;
	Sleep(300);
	ShowMessage("Игра окончена! Ваш рекорд: " + IntToStr(score));
	Form1->StringGrid1->Visible = false;
	Form1->Label1->Visible = false;
}
//функция описыающая движение змеи
void move_snake()
{
	head_x += dx; head_y += dy;
	 for (int i = len - 1; i > 0; i--) {
        snake_dir[i][0] = snake_dir[i - 1][0];
		snake_dir[i][1] = snake_dir[i - 1][1];
	}
	snake_dir[0][0] = dx;
	snake_dir[0][1] = dy;


	for (int i = len - 1; i > 0; i--) {
		snake[i][0] = snake[i - 1][0];
		snake[i][1] = snake[i - 1][1];
	}

	snake[0][0] = head_x;
	snake[0][1] = head_y;

	tail_dx = snake[len - 1][0] - snake[len - 2][0];
	tail_dy = snake[len - 1][1] - snake[len - 2][1];
}
//функция рисующая символы для игры
void draw_snake()
{
for (int i = 1; i < len - 1; i++) {
	if ((snake_dir[i][0] != 0 && snake_dir[i - 1][1] != 0) || (snake_dir[i][1] != 0 && snake_dir[i - 1][0] != 0))
	{
		if ((snake_dir[i][0] == 1 && snake_dir[i - 1][1] == -1) || (snake_dir[i][1] == 1 && snake_dir[i - 1][0] == -1))
		{
			Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "TL";
		} else if ((snake_dir[i][0] == -1 && snake_dir[i - 1][1] == -1) || (snake_dir[i][1] == 1 && snake_dir[i - 1][0] == 1))
		{
			Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "TR";
		} else if ((snake_dir[i][0] == -1 && snake_dir[i - 1][1] == 1) || (snake_dir[i][1] == -1 && snake_dir[i - 1][0] == 1))
		{
			Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "BR";
		} else if ((snake_dir[i][0] == 1 && snake_dir[i - 1][1] == 1) || (snake_dir[i][1] == -1 && snake_dir[i - 1][0] == -1))
		{
			Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "BL";
        }
    } else {
		if (snake_dir[i][0] != 0) {
			Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "Sh";
        } else {
            Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "Sv";
        }
    }
}

	Form1->StringGrid1->Cells[head_x][head_y] = "H"; // Рисуем голову отдельно
	Form1->StringGrid1->Cells[snake[len - 1][0]][snake[len - 1][1]] = "T"; // Рисуем хвост отдельно

}
//обновление
void clear_snake() {
	for (int i = 0; i < len; i++) {
		Form1->StringGrid1->Cells[snake[i][0]][snake[i][1]] = "";
	}
	//Form1->StringGrid1->Cells[head_x][head_y] = "";
	//Form1->StringGrid1->Cells[snake[len - 1][0]][snake[len - 1][1]] = "";
}
//функция проверяющая столкновения с зоной игры и с телом змеи
void check_collision() {
	if (head_x < 0 || head_x >= Form1->StringGrid1->ColCount || head_y < 0 || head_y >= Form1->StringGrid1->RowCount) {
		game_over();
	}
	for (int i = 1; i < len; i++) {
		if (snake[i][0] == head_x && snake[i][1] == head_y) {
			game_over();
		}
	}
}
//функция описывающая поедание яблока
void eat_apple() {
if (head_x == apple_x && head_y == apple_y) {
		PlaySound(TEXT(".\\assets\\apple.wav"), NULL, SND_FILENAME | SND_ASYNC);
		spawn_apple();
		score++;
		Form1->Label1->Caption = "Очки: " + IntToStr(score);
		// Добавляем новый элемент в конец змейки
		snake[len][0] = snake[len - 1][0];
		snake[len][1] = snake[len - 1][1];
		len++;
	}
}
//очистка поля игры чтобы очистить для новой игровой сессии
void clear_field() {
	for (int i = 0; i < Form1->StringGrid1->ColCount; i++) {
		for (int j = 0; j < Form1->StringGrid1->RowCount; j++) {
			Form1->StringGrid1->Cells[i][j] = "";
		}
	}
}
//таймер
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	canChangeDirection = true; // Разрешаем изменение направления
	KillTimer(Handle, 1); // Останавливаем таймер
	if (isPaused) return;
	StringGrid1->DoubleBuffered = true;
	clear_snake();
	move_snake();
	check_collision();
	eat_apple();
	if (!gameover)
		draw_snake();
}
 //управление
 void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 if (!canChangeDirection) return; // Если изменение направления заблокировано, выходим из функции
	 switch (Key) {
		case VK_UP: if (dy != 1) { dx = 0; dy = -1; } break;
		case VK_DOWN: if (dy != -1) { dx = 0; dy = 1; } break;
		case VK_LEFT: if (dx != 1) { dx = -1; dy = 0; } break;
		case VK_RIGHT: if (dx != -1) { dx = 1; dy = 0; } break;
		case VK_SPACE: if (!gameover) { isPaused = !isPaused; } break;
	}
	canChangeDirection = false; // Блокируем изменение направления
	SetTimer(Handle, 1, 100, NULL); // Устанавливаем таймер на 100 мс
}
// кнопка меню начать игру
void __fastcall TForm1::StartGameClick(TObject *Sender)
{
 MainMenu1->Items->Items[0]->Visible = false;
 MainMenu1->Items->Items[1]->Visible = false;
 Image1->Visible = false;
 Label1->Visible = true;
 Label1->Caption = "Очки: " + IntToStr(score);
 StringGrid1->Visible = true;
 Sleep(350);
 clear_field();
 new_game();
}
//при создании формы применяются первоначальные параметры
 void __fastcall TForm1::FormCreate(TObject *Sender)
{
Timer1->Interval = 350;
Timer1->Enabled = false;
StringGrid1->Visible = false;
Label1->Visible = false;

}
//в этой функции идёт отрисовка изображений для игры
void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
	  TRect &Rect, TGridDrawState State)
{
TPicture *Picture = new TPicture;
try
{ // Проверяем, что находится в ячейке
	AnsiString cellContent = StringGrid1->Cells[ACol][ARow];
	int imageIndex = -1; // По умолчанию изображение отсутствует
	if (cellContent == "A") {// Загружаем изображение яблока
	imageIndex = 0;}
	else if (cellContent == "Sh")
	{imageIndex = 1;}

	else if (cellContent == "Sv")
	{imageIndex = 2;}

	else if (cellContent == "H"){
// Загружаем изображение головы змеи в зависимости от направления
	if (dx == 0 && dy == -1){
	imageIndex = 3;
	} else if (dx == 0 && dy == 1) {
	imageIndex = 4;
	} else if (dx == -1 && dy == 0) {
	imageIndex = 5;
	} else if (dx == 1 && dy == 0) {
	imageIndex = 6;
	}
	}

	else if (cellContent == "T")
	{
	// Загружаем изображение хвоста змеи в зависимости от направления
	if (tail_dx == 0 && tail_dy == -1) {
	imageIndex = 7;
	} else if (tail_dx == 0 && tail_dy == 1) {
	imageIndex = 8;
	} else if (tail_dx == -1 && tail_dy == 0) {
	imageIndex = 9;
	} else if (tail_dx == 1 && tail_dy == 0) {
	imageIndex = 10;
	}
	}
	else if (cellContent == "BL") {
	imageIndex = 11;}
	else if (cellContent == "BR") {
	imageIndex = 12;}
	else if (cellContent == "TL") {
	imageIndex = 13;
	} else if (cellContent == "TR") {
	imageIndex = 14;
	}
	else {
	imageIndex = 15;
	}
	// Рисуем изображение в ячейке
	ImageList1->GetBitmap(imageIndex, Picture->Bitmap);
	StringGrid1->Canvas->StretchDraw(Rect, Picture->Graphic);
}
	catch(EFOpenError &e)
	{
		MessageDlg("Ошибка доступа к изображениям змеи",
		mtWarning, TMsgDlgButtons() << mbOK<<mbHelp,0);
		return;
	}
	delete Picture;
}

//Выход
 void __fastcall TForm1::ExitClick(TObject *Sender)
{Close();}
//Уровни сложности
//лёгкая
void __fastcall TForm1::EasyClick(TObject *Sender)
{ Timer1->Interval = 350;
  Easy->Checked = true;
  Medium->Checked = false;
  Hard->Checked = false;}
//средняя
void __fastcall TForm1::MediumClick(TObject *Sender)
{ Timer1->Interval = 300;
   Medium->Checked = true;
   Easy->Checked = false;
   Hard->Checked = false;}
//тяжёлый
void __fastcall TForm1::HardClick(TObject *Sender)
{Timer1->Interval = 250;
   Hard->Checked = true;
   Easy->Checked = false;
   Medium->Checked = false;}
// Открытие второго окна "Как играть?"
void __fastcall TForm1::HowToPlayClick(TObject *Sender)
{Form2->ShowModal();}
//Открытие третьего окна "Об авторе"
void __fastcall TForm1::AboutClick(TObject *Sender)
{ Form3->ShowModal();}

