#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include <string>
#include "projeteis.h"
#include <sstream>
using namespace std;

// -------- VARIAVEIS GLOBAIS DE PROJETEIS E ATIRADORES----------
const int NUM_BALAS_C = 10;
const int NUM_BALAS_B = 10;
const int NUM_BALAS_E = 10;
const int NUM_BALAS_D = 10;
const int NUM_ATIRADOR = 10;
const int NUM_BALASATIRADOR = 5;
const int NUM_PERSONAGEM = 1;

//DEFINICAO DO FPS
const float FPS = 60.0;
//ALTURA E LARGURA DA TELA
#define largura 640				
#define altura 480

int main()
{
	//DEFINICAO DA VARIAVEL DE DISPLAY DO ALLEGRO	
	ALLEGRO_DISPLAY* display;

	//DEFINICAO DAS TECLAS DE DIRECAO, PROJETEIS E DEMAIS TECLAS DO JOGO
	enum Direction { DOWN = 0, LEFT = 3, RIGHT = 6, UP = 9};
	enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA, ENTER, UM, DOIS, TRES, F1, F2, F3, F4, F5};

	//INICIALIZACAO DA TELA
	if (!al_init())
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	display = al_create_display(largura, altura);
	al_set_window_title(display, "PI2-BCC-ATENAS");

	if (!display)
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	al_set_window_position(display, 200, 200);

	//VARIAVEIS DE SUPORTE
	bool done = false, draw = true, active = false, gameover = false, proximafase = false, iniciar = false, fimdejogo = false, respondido[8] = { false, false, false, false, false, false, false, false };
	bool vermelho = false, verde = false, azul = false, amarelo = false, marrom = false;
	int x = 10, y = 10, moveSpeed = 5, pontos = 0, contador = 0, r = 0, g = 138, b = 200, r2 = 0, g2 = 138, b2 = 200, corBala = 4, corAtirador = 4;
	int dir = DOWN, dir2 = 1, dir3 = 0, prevDir = 0, fase = 1;
	bool tiros[] = {false, false, false, false, false, false, false, false, false, false, false, false, false};
	bool historia = true, comandos = true, inicio = false;
	
	// ------------ INICIALIZACAO DOS PROTOTIPOS DOS ATIRADORES E PROJETEIS --------------
	Atirador personagem[NUM_PERSONAGEM];
	Projeteis balas_c[NUM_BALAS_C];
	Projeteis balas_b[NUM_BALAS_B];
	Projeteis balas_e[NUM_BALAS_E];
	Projeteis balas_d[NUM_BALAS_D];
	Atirador atirador[NUM_ATIRADOR];
	Projeteis balas[NUM_BALASATIRADOR];


	// -------- FUNCOES INICIAIS DOS PROJETEIS E ATIRADORES ---------------
	InitAtirador(personagem, NUM_PERSONAGEM, "personagem", 5, 5.0, 0);
	InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
	InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
	InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
	InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
	InitAtirador(atirador, NUM_ATIRADOR-9, "atirador", 1, 0, corAtirador);
	InitBalas(balas, NUM_BALASATIRADOR, "atirador", 1, corBala);

	
	//INICIALIZACAO DOS ADDONS DO ALLEGRO
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_reserve_samples(10);

	//------------INICIALIZACAO DOS EVENTOS DO ALLEGRO---------------
	
	//TEXTO
	ALLEGRO_FONT* font = al_load_font("fontes/Minecraft.ttf", 25, NULL);
	ALLEGRO_FONT* font2 = al_load_font("fontes/Minecraft.ttf", 12, NULL);

	//BITMAP DO PERSONAGEM
	ALLEGRO_BITMAP* playerWalk[12];
	for (int i = 0; i < 12; i++)
	{
		std::stringstream str;
		str << "sprites/" << i + 1 << ".png";
		playerWalk[i] = al_load_bitmap(str.str().c_str());
	}

	//BITMAPS DO MAPA, IMAGENS E PERGUNTAS
	ALLEGRO_BITMAP* mapa = al_load_bitmap("mapas/bosque1.png");
	ALLEGRO_BITMAP* lixeira = al_load_bitmap("imagens/azul.png");
	ALLEGRO_BITMAP* enter = al_load_bitmap("imagens/enter.png");
	ALLEGRO_BITMAP* coracao = al_load_bitmap("inimigos/coracao_vermelho.png");
	ALLEGRO_BITMAP* imagem1 = al_load_bitmap("imagens/inicio.png");
	ALLEGRO_BITMAP* imagem2 = al_load_bitmap("imagens/historia.png");
	ALLEGRO_BITMAP* imagem3 = al_load_bitmap("imagens/gameover.png");
	ALLEGRO_BITMAP* imagem4 = al_load_bitmap("imagens/comandos.png");
	ALLEGRO_BITMAP* imagem5 = al_load_bitmap("imagens/congratulations.png");
	ALLEGRO_BITMAP* pergunta = al_load_bitmap("perguntas/1/1.png");
	ALLEGRO_BITMAP* pergunta2 = al_load_bitmap("perguntas/2/1.png");
	ALLEGRO_BITMAP* pergunta3 = al_load_bitmap("perguntas/3/1.png");
	ALLEGRO_BITMAP* pergunta4 = al_load_bitmap("perguntas/4/1.png");
	ALLEGRO_BITMAP* pergunta5 = al_load_bitmap("perguntas/5/1.png");
	ALLEGRO_BITMAP* pergunta6 = al_load_bitmap("perguntas/6/1.png");
	ALLEGRO_BITMAP* pergunta7 = al_load_bitmap("perguntas/7/1.png");
	ALLEGRO_BITMAP* pergunta8 = al_load_bitmap("perguntas/8/1.png");

	//BITMAP DO INIMIGO
	ALLEGRO_BITMAP* enemy = al_load_bitmap("inimigos/papel.png");

	//EVENTO DE TECLADO
    ALLEGRO_KEYBOARD_STATE keyState;

	//TIMERS
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* timer2 = al_create_timer(1);

	//FILA DE EVENTOS
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//TRILHA SONORA
	ALLEGRO_SAMPLE* trilha_sonora = NULL;
	ALLEGRO_SAMPLE* projeteis_lancados = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_projeteis_lancados = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;
	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);
	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 0.3);

	ALLEGRO_SAMPLE* sound_projetil = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_sound_projetil = NULL;

	sound_projetil = al_load_sample("sound_bullet.ogg");

	inst_sound_projetil = al_create_sample_instance(sound_projetil);

	al_attach_sample_instance_to_mixer(inst_sound_projetil, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_sound_projetil, ALLEGRO_PLAYMODE_BIDIR);
	al_set_sample_instance_gain(inst_sound_projetil, 1);

	//LOOP CONTENDO A LOGICA DO JOGO
	srand(time(NULL));
	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(imagem1, 0, 0, NULL);
	al_flip_display();

	while (!done)
	{
		int a = al_get_timer_count(timer2);
		//cout << a << endl;
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		al_get_keyboard_state(&keyState);
		al_play_sample_instance(inst_trilha_sonora);

		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		//IMPLEMENTACAO DO CODIGO QUE CADA TECLA ACIONA AO SER PRESSIONADA
		else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				tiros[DIREITA] = true;
				AtiraBalas(balas_d, NUM_BALAS_D, personagem, NUM_ATIRADOR, tiros, "personagem", 2);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_LEFT:
				tiros[ESQUERDA] = true;
				AtiraBalas(balas_e, NUM_BALAS_E, personagem, NUM_ATIRADOR, tiros, "personagem", 1);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_DOWN:
				tiros[BAIXO] = true;
				AtiraBalas(balas_b, NUM_BALAS_B, personagem, NUM_ATIRADOR, tiros, "personagem", 4);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_UP:
				tiros[CIMA] = true;
				AtiraBalas(balas_c, NUM_BALAS_C, personagem, NUM_ATIRADOR, tiros, "personagem", 3);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_ENTER:
				tiros[ENTER] = true;
				break;
			case ALLEGRO_KEY_1:
				tiros[UM] = true;
				break;
			case ALLEGRO_KEY_2:
				tiros[DOIS] = true;
				break;
			case ALLEGRO_KEY_3:
				tiros[TRES] = true;
				break;
			case ALLEGRO_KEY_F1:
				tiros[F1] = true;
				r = 0;
				g = 177;
				b = 78;
				verde = true;
				corBala = 1;
				break;
			case ALLEGRO_KEY_F2:
				tiros[F2] = true;
				r = 195;
				g = 52;
				b = 48;
				vermelho = true;
				corBala = 2;
				break;
			case ALLEGRO_KEY_F3:
				tiros[F3] = true;
				r = 249;
				g = 189;
				b = 0;
				amarelo = true;
				corBala = 3;
				break;
			case ALLEGRO_KEY_F4:
				tiros[F4] = true;
				r = 0;
				g = 138;
				b = 200;
				azul = true;
				corBala = 4;
				break;
			case ALLEGRO_KEY_F5:
				tiros[F5] = true;
				r = 50;
				g = 30;
				b = 0;
				marrom = true;
				corBala = 5;
				break;
			}
		}


		else if (events.type == ALLEGRO_EVENT_TIMER)
		{
			active = true;
				
			//ATUALIZACAO DOS PROJETEIS NA TELA QUANDO DISPARADOS
				tiros[ENTER] = false;
				if (tiros[DIREITA])
					AtualizaBalas(balas_d, NUM_BALAS_D, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[ESQUERDA])
					AtualizaBalas(balas_e, NUM_BALAS_E, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[BAIXO])
					AtualizaBalas(balas_b, NUM_BALAS_B, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[CIMA])
					AtualizaBalas(balas_c, NUM_BALAS_C, personagem, NUM_PERSONAGEM, largura, altura);

				if (!gameover)
				{
					//ATUALIZAÇAO DOS PROJETEIS, PERSONAGEM E INIMIGOS NA TELA
					AtiraBalas(balas, NUM_BALASATIRADOR, atirador, NUM_ATIRADOR, tiros, "atirador", 0);
					AtualizaBalas(balas, NUM_BALASATIRADOR, personagem, NUM_PERSONAGEM, largura, altura);
					LiberaTiros(atirador, NUM_ATIRADOR, "atirador");
					move_personagem(keyState, personagem, NUM_PERSONAGEM, altura, largura, &draw);
					LiberaTiros(personagem, NUM_ATIRADOR, "personagem");
					AtualizaAtirador(atirador, altura, largura, NUM_ATIRADOR);

					//COLISAO DOS PROJETEIS COM O INIMIGO E PERSONAGEM
					BalaColidida(balas, NUM_BALASATIRADOR, personagem, NUM_PERSONAGEM, "atirador", &pontos, corBala, corAtirador);
					BalaColidida(balas_b, NUM_BALAS_B, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_c, NUM_BALAS_C, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_d, NUM_BALAS_D, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_e, NUM_BALAS_E, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					
				}
				
		}
		//LOGICA DE DESENHOS DO JOGO
		if (draw)
		{	
			//DESENHO DAS TELAS INICIAIS DE HISTORIA E COMANDOS
			cout << a << endl;
			if (tiros[ENTER])
			{
				al_start_timer(timer2);
			}

			if (a>1 && a<20)
			{
				//al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(imagem2, 0, 0, NULL);
				//al_draw_textf(font2, al_map_rgb(255, 255, 255), 400, 450, NULL, "APERTE ENTER PARA CONTINUAR");
				al_flip_display();
			}

			if (a>20 && inicio == false)
			{
				al_clear_to_color(al_map_rgb(27, 111, 27));
				al_draw_bitmap(imagem4, 0, 0, NULL);
				al_draw_textf(font2, al_map_rgb(255, 255, 255), 400, 450, NULL, "APERTE ENTER PARA CONTINUAR");
				al_flip_display();
				//inicio = true;
				if (tiros[ENTER])
				{
					inicio = true;
				}
			}

			if (inicio)
			{
				iniciar = true;
			}
				

			if (iniciar)
			{
				al_draw_bitmap(mapa, 0, 0, NULL);

				if (!gameover && !proximafase)
				{
					//DESENHO DOS PROJETEIS, INIMIGOS E PERSONAGEM
					DesenhaBalas(balas_c, NUM_BALAS_C, 5, r, g, b);
					DesenhaBalas(balas_b, NUM_BALAS_B, 5, r, g, b);
					DesenhaBalas(balas_e, NUM_BALAS_E, 5, r, g, b);
					DesenhaBalas(balas_d, NUM_BALAS_D, 5, r, g, b);
					DesenhaAtirador(enemy, playerWalk, atirador, NUM_ATIRADOR, "atirador", font2, coracao);
					DesenhaBalas(balas, NUM_BALASATIRADOR, 8, r2, g2, b2);
					DesenhaAtirador(enemy, playerWalk, personagem, NUM_PERSONAGEM, "personagem", font2, coracao);

					//DESENHO DA BARRA SUPERIOR DE CONSULTA
					al_draw_textf(font, al_map_rgb(255, 255, 255), 165, 15, NULL, "%d", personagem->vida);
					al_draw_filled_circle(280, 25, 10, al_map_rgb(r, g, b));
					al_draw_bitmap(lixeira, 240, 8, NULL);
					al_draw_textf(font, al_map_rgb(255, 255, 255), 430, 12, NULL, "%d", fase);
					al_draw_textf(font, al_map_rgb(255, 255, 255), 580, 15, NULL, "%d", pontos);

					//ACIONAMENTO DO GAMEOVER
					if (personagem->vida <= 0)
						gameover = true;

					contador = pontos;

					//LOGICA PARA PROSSEGUIMENTO DE FASES
					if (contador == 1 && fase == 1)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 4 && fase == 2)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 7 && fase == 3)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 11 && fase == 4)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 15 && fase == 5)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 20 && fase == 6)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 24 && fase == 7)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 29 && fase == 8)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 30 && fase == 9)
					{
						proximafase = true;
						contador = 0;
					}
				}
				else
				{
					//DESENHO DA TELA DE GAMEOVER
					if (gameover)
					{ 
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(imagem3, 0, 0, NULL);
						al_flip_display();
						//al_draw_textf(font2, al_map_rgb(255, 255, 255), largura / 2, altura / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER: VOCE PERDEU TODAS AS VIDAS, COMECE NOVAMENTE");
						al_draw_textf(font, al_map_rgb(255, 255, 255), largura / 2, 360, ALLEGRO_ALIGN_CENTRE, "SEUS PONTOS FORAM: %d", pontos);
					}
					//DESENHO DA PERGUNTA 1
					if (proximafase && fase == 1)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[0])
						{
							al_clear_to_color(al_map_rgb(27, 111, 27));
							tiros[UM] = false;
							respondido[0] = true;
							personagem->vida++;
							pergunta = al_load_bitmap("perguntas/1/4.png");
							//al_draw_bitmap(enter, 400, 450, NULL);
							//al_flip_display();
						}
						if (tiros[DOIS] && !respondido[0])
						{
							tiros[DOIS] = false;
							respondido[0] = true;
							pergunta = al_load_bitmap("perguntas/1/2.png");
						}
						if (tiros[TRES] && !respondido[0])
						{
							tiros[TRES] = false;
							respondido[0] = true;
							pergunta = al_load_bitmap("perguntas/1/3.png");
						}
						
					}
					//DESENHO DA PERGUNTA 2
					if (proximafase && fase == 2)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta2, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[1])
						{
							tiros[UM] = false;
							respondido[1] = true;
							pergunta2 = al_load_bitmap("perguntas/2/2.png");
						}
						if (tiros[DOIS] && !respondido[1])
						{
							tiros[DOIS] = false;
							respondido[1] = true;
							pergunta2 = al_load_bitmap("perguntas/2/3.png");
						}
						if (tiros[TRES] && !respondido[1])
						{
							tiros[TRES] = false;
							respondido[1] = true;
							personagem->vida++;
							pergunta2 = al_load_bitmap("perguntas/2/4.png");
						}
					}
					//DESENHO DA PERGUNTA 3
					if (proximafase && fase == 3)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta3, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[2])
						{
							tiros[UM] = false;
							respondido[2] = true;
							pergunta3 = al_load_bitmap("perguntas/3/2.png");
						}
						if (tiros[DOIS] && !respondido[2])
						{
							tiros[DOIS] = false;
							respondido[2] = true;
							personagem->vida++;
							pergunta3 = al_load_bitmap("perguntas/3/4.png");
						}
						if (tiros[TRES] && !respondido[2])
						{
							tiros[TRES] = false;
							respondido[2] = true;
							pergunta3 = al_load_bitmap("perguntas/3/3.png");
						}

					}
					//DESENHO DA PERGUNTA 4
					if (proximafase && fase == 4)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta4, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[3])
						{
							tiros[UM] = false;
							respondido[3] = true;
							pergunta4 = al_load_bitmap("perguntas/4/2.png");
						}
						if (tiros[DOIS] && !respondido[3])
						{
							tiros[DOIS] = false;
							respondido[3] = true;
							pergunta4 = al_load_bitmap("perguntas/4/3.png");
						}
						if (tiros[TRES] && !respondido[3])
						{
							tiros[TRES] = false;
							respondido[3] = true;
							personagem->vida++;
							pergunta4 = al_load_bitmap("perguntas/4/4.png");
						}

					}
					//DESENHO DA PERGUNTA 5
					if (proximafase && fase == 5)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta5, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[4])
						{
							tiros[UM] = false;
							respondido[4] = true;
							pergunta5 = al_load_bitmap("perguntas/5/2.png");
						}
						if (tiros[DOIS] && !respondido[4])
						{
							tiros[DOIS] = false;
							respondido[4] = true;
							personagem->vida++;
							pergunta5 = al_load_bitmap("perguntas/5/4.png");
						}
						if (tiros[TRES] && !respondido[4])
						{
							tiros[TRES] = false;
							respondido[4] = true;
							pergunta5 = al_load_bitmap("perguntas/5/3.png");
						}

					}
					//DESENHO DA PERGUNTA 6
					if (proximafase && fase == 6)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta6, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[5])
						{
							tiros[UM] = false;
							respondido[5] = true;
							personagem->vida++;
							pergunta6 = al_load_bitmap("perguntas/6/4.png");
						}
						if (tiros[DOIS] && !respondido[5])
						{
							tiros[DOIS] = false;
							respondido[5] = true;
							pergunta6 = al_load_bitmap("perguntas/6/2.png");
						}
						if (tiros[TRES] && !respondido[5])
						{
							tiros[TRES] = false;
							respondido[5] = true;
							pergunta6 = al_load_bitmap("perguntas/6/3.png");
						}

					}
					//DESENHO DA PERGUNTA 7
					if (proximafase && fase == 7)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta7, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[6])
						{
							tiros[UM] = false;
							respondido[6] = true;
							pergunta7 = al_load_bitmap("perguntas/7/2.png");
						}
						if (tiros[DOIS] && !respondido[6])
						{
							tiros[DOIS] = false;
							respondido[6] = true;
							personagem->vida++;
							pergunta7 = al_load_bitmap("perguntas/7/4.png");
						}
						if (tiros[TRES] && !respondido[6])
						{
							tiros[TRES] = false;
							respondido[6] = true;
							pergunta7 = al_load_bitmap("perguntas/7/3.png");
						}

					}
					//DESENHO DA PERGUNTA 8
					if (proximafase && fase == 8)
					{
						al_clear_to_color(al_map_rgb(27, 111, 27));
						al_draw_bitmap(pergunta8, 0, 0, NULL);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 450, NULL, "RESPONDA E DEPOIS APERTE ENTER PARA CONTINUAR");
						//al_flip_display();
						if (tiros[UM] && !respondido[7])
						{
							tiros[UM] = false;
							respondido[7] = true;
							pergunta8 = al_load_bitmap("perguntas/8/2.png");
						}
						if (tiros[DOIS] && !respondido[7])
						{
							tiros[DOIS] = false;
							respondido[7] = true;
							pergunta8 = al_load_bitmap("perguntas/8/3.png");
						}
						if (tiros[TRES] && !respondido[7])
						{
							tiros[TRES] = false;
							respondido[7] = true;
							personagem->vida++;
							pergunta8 = al_load_bitmap("perguntas/8/4.png");
						}

					}
					//DESENHO DA TELA DE GAME WIN
					if (proximafase && fase == 9)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(imagem5, 0, 0, NULL);
						al_flip_display();

					}

					if (tiros[ENTER])
					{
						//SE GAME OVER A TELA FECHA
						if (gameover)
						{
							done = true;
							/*
							fase = 1;
							mapa = al_load_bitmap("mapa.png");
							enemy = al_load_bitmap("trash.png");
							InitAtirador(personagem, NUM_PERSONAGEM, "personagem", 5, 5.0);
							InitBalas(balas_c, NUM_BALAS_C, "personagem");
							InitBalas(balas_b, NUM_BALAS_B, "personagem");
							InitBalas(balas_e, NUM_BALAS_E, "personagem");
							InitBalas(balas_d, NUM_BALAS_D, "personagem");
							InitAtirador(atirador, NUM_ATIRADOR-7, "atirador", 1, 0.5);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador");
							gameover = false;
							tiros[ENTER] = false;
							pontos = 0;*/
						}
						// INICIALIZACAO DA FASE 2
						if (proximafase && fase == 1)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 4;
							fase++;
							mapa = al_load_bitmap("mapas/bosque2.png");
							enemy = al_load_bitmap("inimigos/papel2.png");
							//lixeira = al_load_bitmap("imagens/azul.png");
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 7, "atirador", 2, 0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 3
						if (proximafase && fase == 2)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 3;
							fase++;
							mapa = al_load_bitmap("mapas/cidade1.png");
							enemy = al_load_bitmap("inimigos/metal.png");
							lixeira = al_load_bitmap("imagens/amarela.png");
							r2 = 249, g2 = 189, b2 = 0;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 7, "atirador", 2, 1.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 4
						if (proximafase && fase == 3)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 3;
							fase++;
							mapa = al_load_bitmap("mapas/cidade2.png");
							enemy = al_load_bitmap("inimigos/metal2.png");
							lixeira = al_load_bitmap("imagens/amarela.png");
							r2 = 249, g2 = 189, b2 = 0;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 3, 1.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 5
						if (proximafase && fase == 4)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 2;
							fase++;
							mapa = al_load_bitmap("mapas/deserto1.png");
							enemy = al_load_bitmap("inimigos/plastico.png");
							lixeira = al_load_bitmap("imagens/vermelha.png");
							r2 = 195, g2 = 52, b2 = 48;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 3, 1.5, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 6
						if (proximafase && fase == 5)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 2;
							fase++;
							mapa = al_load_bitmap("mapas/deserto2.png");
							enemy = al_load_bitmap("inimigos/plastico2.png");
							lixeira = al_load_bitmap("imagens/vermelha.png");
							r2 = 195, g2 = 52, b2 = 48;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 5, "atirador", 3, 1.5, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 7
						if (proximafase && fase == 6)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 1;
							fase++;
							mapa = al_load_bitmap("mapas/lixao1.png");
							enemy = al_load_bitmap("inimigos/vidro.png");
							lixeira = al_load_bitmap("imagens/verde.png");
							r2 = 0, g2 = 177, b2 = 78;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 4, 2.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 8
						if (proximafase && fase == 7)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 1;
							fase++;
							mapa = al_load_bitmap("mapas/lixao2.png");
							enemy = al_load_bitmap("inimigos/vidro2.png");
							lixeira = al_load_bitmap("imagens/verde.png");
							r2 = 0, g2 = 177, b2 = 78;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 5, "atirador", 4, 2.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// INICIALIZACAO DA FASE 9 (BOSS)
						if (proximafase && fase == 8)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 5;
							fase++;
							mapa = al_load_bitmap("mapas/lixao3.png");
							enemy = al_load_bitmap("inimigos/boss5.png");
							lixeira = al_load_bitmap("imagens/marrom.png");
							r2 = 50, g2 = 30, b2 = 0;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 9, "atirador", 10, 3.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 10, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						// SE GAME WIN A TELA FECHA
						if (proximafase && fase == 9)
						{
							done = true;
						}

					}

				}
				al_flip_display();
			}
		
			
		}
	}

	//LIMPEZA DE MEMORIA
	al_destroy_display(display);
	al_destroy_timer(timer);
	for (int i = 0; i < 12; i++)
	{
		al_destroy_bitmap(playerWalk[i]);
	}
	//al_destroy_bitmap(enemy);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_sample(trilha_sonora);
	al_destroy_sample(projeteis_lancados);
	al_destroy_sample_instance(inst_trilha_sonora);
	al_destroy_sample_instance(inst_projeteis_lancados);

	return 0;

}

/*



*/
