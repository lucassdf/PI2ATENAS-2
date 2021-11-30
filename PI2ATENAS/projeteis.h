#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

enum IDS { JOGADOR, PROJETIL, INIMIGOS, PROJETEISATIRADOR};
enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA };
enum Direction {DOWN = 0, LEFT = 3, RIGHT=6, UP=9};
int index = 0;
int index2 = 0;
int prevDir = 0;

struct Projeteis
{
	int ID;
	int x;
	int y;
	int velocidade;
	int dir;
	bool ativo;
	int cor;
};

struct Atirador {
	int ID;
	float x;
	float y;
	int velocidade;
	int borda_x;
	int borda_y;
	int dir;
	int vida;
	int pontos;
	bool ativo;
	int cor;
};

// ---------- PROT�TIPOS -------------

void AtiraBalas(Projeteis balas[], int tamanho, Atirador atirador[], int tamanho_a, bool tiros[], string tipo, int dir);
void DesenhaBalas(Projeteis balas[], int tamanho, int raio, int rgb1, int rgb2, int rgb3);
void BalaColidida(Projeteis balas[], int b_tamanho, Atirador atirador[], int c_tamanho, string tipo, int *pontos);
void InitAtirador(Atirador atirador[], int tamanho, string tipo, int vida, float velocidade, int corAtirador);
void LiberaTiros(Atirador atirador[], int tamanho, string tipo);
void AtualizaAtirador(Atirador atirador[], float  alt, float larg, int tamanho);
void InitBalas(Projeteis balas[], int tamanho, string tipo, int velocidade, int corBala);
void AtualizaBalas(Projeteis balas[], int tamanho);
void DesenhaAtirador(ALLEGRO_BITMAP* playerWalk, Atirador atirador[], int tamanho, string tipo, int index, ALLEGRO_FONT* font, ALLEGRO_BITMAP* coracao);
void move_personagem(ALLEGRO_KEYBOARD_STATE keyState, Atirador atirador[], int tamanho_a, int alt, int larg, bool* draw, int* prevDir, int index);



//------------------------------------------------------------------------------------------------------------------------------
void move_personagem(ALLEGRO_KEYBOARD_STATE keyState, Atirador atirador[], int tamanho_a, int alt, int larg, bool* draw)
{
	
	
	//LOGICA DE MOVIMENTACAO DO PERSONAGEM
	for (int i = 0; i < tamanho_a; i++)
	{
		prevDir = atirador[i].dir;
		if (atirador[i].ativo)
		{
			if (al_key_down(&keyState, ALLEGRO_KEY_S) && atirador[i].y < alt - 30)
			{
				atirador[i].y += atirador[i].velocidade;
				atirador[i].dir = DOWN;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_W) && atirador[i].y >50)
			{
				atirador[i].y -= atirador[i].velocidade;
				atirador[i].dir = UP;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_D) && atirador[i].x < larg - 30)
			{
				atirador[i].x += atirador[i].velocidade;
				atirador[i].dir = RIGHT;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_A) && atirador[i].x > 0)
			{
				atirador[i].x -= atirador[i].velocidade;
				atirador[i].dir = LEFT;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else
				atirador[i].ativo = false;

			if (atirador[i].ativo)
			{
				index++;
			}
			else
			{
				index = atirador[i].dir + 1;
			}

			if (index >= atirador[i].dir + 3)
				index = atirador[i].dir;

			if (atirador[i].dir != prevDir)
				index = atirador[i].dir;

			

			//*draw = true;
		}
		
	}
}
void InitAtirador(Atirador atirador[], int tamanho, string tipo, int vida, float velocidade, int corAtirador)
{
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++)
		{
			atirador[i].ID = JOGADOR;
			atirador[i].velocidade = velocidade;
			atirador[i].borda_x = 20;
			atirador[i].borda_y = 20;
			atirador[i].vida = vida;
			atirador[i].pontos = 0;
			atirador[i].x = 50;
			atirador[i].y = 50;
			atirador[i].ativo = false;
			atirador[i].dir = 0;
		}
	}
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++)
		{
			atirador[i].ID = INIMIGOS;
			atirador[i].velocidade = velocidade;
			atirador[i].borda_x = 18;
			atirador[i].borda_y = 18;
			atirador[i].vida = vida;
			atirador[i].pontos = 0;
			atirador[i].ativo = false;
			atirador[i].cor = corAtirador;
		}
	}
}
void AtualizaAtirador(Atirador atirador[], float  alt, float larg, int tamanho)
{
	int opcao = 2;
	for (int i = 0; i < tamanho; i++)
	{
		if (atirador[i].ativo && opcao == 1)
		{
			if (atirador[i].dir == 1 && atirador[i].x != 20)
			{
				--atirador[i].x;
			}
			else if (atirador[i].dir == 2 && atirador[i].y != alt - 50)
			{
				++atirador[i].y;
			}
			else if (atirador[i].dir == 3 && atirador[i].x != larg - 50)
			{
				++atirador[i].x;
			}
			else if (atirador[i].dir == 4 && atirador[i].y != 20)
			{
				--atirador[i].y;
			}
			else
			{
				atirador[i].dir = rand() % 4 + 1;
			}

		}
		if (atirador[i].ativo && opcao == 2)
		{
			if (atirador[i].dir == 1 && atirador[i].x >= 40.0 && atirador[i].y >= 70.0)
			{
				atirador[i].x -= atirador[i].velocidade;
				atirador[i].y -= atirador[i].velocidade;
			}
			else if (atirador[i].dir == 2 && atirador[i].x >= 40.0 && atirador[i].y <= alt - 40.0)
			{
				atirador[i].x -= atirador[i].velocidade;
				atirador[i].y += atirador[i].velocidade;
			}
			else if (atirador[i].dir == 3 && atirador[i].x <= larg - 40.0 && atirador[i].y >= 70.0)
			{
				atirador[i].x += atirador[i].velocidade;
				atirador[i].y -= atirador[i].velocidade;
			}
			else if (atirador[i].dir == 4 && atirador[i].x <= larg - 40.0 && atirador[i].y <= alt - 40.0)
			{

				atirador[i].x += atirador[i].velocidade;
				atirador[i].y += atirador[i].velocidade;
			}
			else
			{
				atirador[i].dir = rand() % 4 + 1;
			}
		}

	}
}
void DesenhaAtirador(ALLEGRO_BITMAP* enemy, ALLEGRO_BITMAP* playerWalk[12], Atirador atirador[], int tamanho, string tipo, ALLEGRO_FONT* font, ALLEGRO_BITMAP* coracao)
{

	//ALLEGRO_BITMAP* enemy = al_load_bitmap("trash.png");
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (atirador[i].ativo)
			{
				al_draw_filled_circle(atirador[i].x, atirador[i].y, 20, al_map_rgba(0,0, 0, 0));
				al_draw_bitmap(enemy, atirador[i].x - 16, atirador[i].y - 16, NULL);
				al_draw_textf(font, al_map_rgb(255, 255, 255), atirador[i].x-10, atirador[i].y+19, NULL, "%d", atirador[i].vida);
				al_draw_bitmap(coracao, atirador[i].x, atirador[i].y + 18, NULL);
			}
		}
	}
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (atirador[i].ativo)
			{
				//al_draw_bitmap(enemy, atirador[i].x, atirador[i].y, NULL);
				al_draw_filled_circle(atirador[i].x+20, atirador[i].y+20, 16, al_map_rgba(0,0, 0, 0));
				al_draw_bitmap(playerWalk[index], atirador[i].x, atirador[i].y, NULL);
				al_convert_mask_to_alpha(playerWalk[index], al_map_rgb(0, 0, 0));
				//al_draw_filled_rectangle(atirador[i].x, atirador[i].y, atirador[i].x+32, atirador[i].y+32, al_map_rgb(0, 128, 0));
				//al_draw_bitmap_region(player, *sourceX, *sourceY * al_get_bitmap_height(player) / 4, 32, 32, atirador[i].x, atirador[i].y, NULL);
			}
		}
	}

}
void LiberaTiros(Atirador atirador[], int tamanho, string tipo)
{
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!atirador[i].ativo && atirador[i].vida > 0)
			{
				atirador[i].x = rand() % 500 + 100;
				atirador[i].y = rand() % 350 + 100;
				atirador[i].ativo = true;

				/*if (rand() % 500 == 0)
				{
					atirador[i].x = 1;
					atirador[i].y = 5;
					atirador[i].ativo = true;
					break;
				}*/
			}
		}
	}
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!atirador[i].ativo && atirador[i].vida > 0)
			{
				//atirador[i].x = 20;
				//atirador[i].y = 20;
				atirador[i].ativo = true;

				/*if (rand() % 500 == 0)
				{
					atirador[i].x = 1;
					atirador[i].y = 5;
					atirador[i].ativo = true;
					break;
				}*/
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------

void BalaColidida(Projeteis balas[], int b_tamanho, Atirador atirador[], int c_tamanho, string tipo, int *pontos, int corBala, int corAtirador)
{
	if (tipo == "personagem" && corBala == corAtirador)
	{
		for (int i = 0; i < b_tamanho; i++)
		{
			if (balas[i].ativo)
			{
				for (int j = 0; j < c_tamanho; j++)
				{
					if (atirador[j].ativo)
					{
						if (balas[i].x > (atirador[j].x - atirador[j].borda_x) &&
							balas[i].x < (atirador[j].x + atirador[j].borda_x) &&
							balas[i].y >(atirador[j].y - atirador[j].borda_y) &&
							balas[i].y < (atirador[j].y + atirador[j].borda_y))
						{
							balas[i].ativo = false;
							--atirador[j].vida;
							if (atirador[j].vida <= 0)
							{
								atirador[j].ativo = false;
								++* pontos;
								/*if (tipo == "atirador")
								{
									al_show_native_message_box(display, "Error", "GAME OVER: VOCE MORREU", "JOGUE NOVAMENTE", NULL, NULL);
									*done = true;
								}
								if (tipo == "personagem" && j == c_tamanho)
								{
									al_show_native_message_box(display, "PARABÉNS", "VOCE DERROTOU TODOS OS INIMIGOS", "PODE PROSSEGUIR SUA JORNADA", NULL, NULL);
									*done = true;
								}*/
							}

						}
					}
				}
			}
		}
	}

	if (tipo == "atirador")
	{
		for (int i = 0; i < b_tamanho; i++)
		{
			if (balas[i].ativo)
			{
				for (int j = 0; j < c_tamanho; j++)
				{
					if (atirador[j].ativo)
					{
						if (balas[i].x > (atirador[j].x - atirador[j].borda_x) &&
							balas[i].x < (atirador[j].x + atirador[j].borda_x) &&
							balas[i].y >(atirador[j].y - atirador[j].borda_y) &&
							balas[i].y < (atirador[j].y + atirador[j].borda_y))
						{
							balas[i].ativo = false;
							--atirador[j].vida;
							if (atirador[j].vida <= 0)
							{
								atirador[j].ativo = false;
								++* pontos;
								/*if (tipo == "atirador")
								{
									al_show_native_message_box(display, "Error", "GAME OVER: VOCE MORREU", "JOGUE NOVAMENTE", NULL, NULL);
									*done = true;
								}
								if (tipo == "personagem" && j == c_tamanho)
								{
									al_show_native_message_box(display, "PARABÉNS", "VOCE DERROTOU TODOS OS INIMIGOS", "PODE PROSSEGUIR SUA JORNADA", NULL, NULL);
									*done = true;
								}*/
							}

						}
					}
				}
			}
		}
	}
}
void AtiraBalas(Projeteis balas[], int tamanho, Atirador atirador[], int tamanho_a, bool tiros[], string tipo, int dir)
{
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!balas[i].ativo)
			{
				balas[i].x = atirador[i].x+16;
				balas[i].y = atirador[i].y+16;
				balas[i].ativo = true;
				balas[i].dir = dir;
				break;
			}

		}
	}
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!balas[i].ativo && atirador[i].vida > 0)
			{
				for (int j = 0; j < tamanho_a; j++)
				{
					balas[i].x = atirador[i].x;
					balas[i].y = atirador[i].y;
					balas[i].dir = rand() % 4 + 1;
					balas[i].ativo = true;
					break;
				}
			}

		}
	}

}
void InitBalas(Projeteis balas[], int tamanho, string tipo, int velocidade, int corBala)
{
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++) {
			balas[i].ID = PROJETEISATIRADOR;
			balas[i].velocidade = velocidade;
			balas[i].ativo = false;
		}
	}
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++) {
			balas[i].ID = PROJETIL;
			balas[i].velocidade = velocidade;
			balas[i].ativo = false;
			balas[i].cor = corBala;
		}
	}
}
void AtualizaBalas(Projeteis balas[], int tamanho_b, Atirador atirador[], int c_tamanho, float larg, float alt)
{
	for (int i = 0; i < tamanho_b; i++)
	{
		if (balas[i].ativo)
		{
			for (int j = 0; j < c_tamanho; j++)
			{
				if (atirador[j].ativo)
				{
					//balas[i].dir = 4;
					if (balas[i].dir == 1)
						balas[i].x -= balas[i].velocidade;
					else if (balas[i].dir == 2)
						balas[i].x += balas[i].velocidade;
					else if (balas[i].dir == 3)
						balas[i].y -= balas[i].velocidade;
					else if (balas[i].dir == 4)
						balas[i].y += balas[i].velocidade;

					if (balas[i].x < 10 || balas[i].x > larg || balas[i].y < 60 || balas[i].y > alt || !atirador[j].ativo)
					{
						balas[i].ativo = false;
					}
				}
			}
		}

	}
}
void DesenhaBalas(Projeteis balas[], int tamanho, int raio, int rgb1, int rgb2, int rgb3)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			al_draw_filled_circle(balas[i].x, balas[i].y, raio, al_map_rgb(rgb1, rgb2, rgb3));
		}
	}
}
