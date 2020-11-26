#include <graphics.h>
#include <bits/stdc++.h>

#define delay_ms(t) \
[](const int &msTime) { \
	int fpsTime = msTime * FPS / 1000; \
	while (fpsTime-- && is_run()) \
		delay_fps(FPS); \
} (t)
// �� delay_ms ת��Ϊ delay_fps (��ΪҪ�ж� is_run ���Բ���ֱ�� delay_ms)
// �� lambda ���Ա����������

#define BACKGROUND_DAY		0
#define BACKGROUND_NIGHT	1
#define BIRD_UP				2
#define BIRD_HORIZONTAL		3
#define BIRD_DOWN			4
#define BOARD				5
#define BUTTON_LIGHT		6
#define BUTTON_DARK			7
#define GROUND				8
#define INSTRUCTION			9
#define SILVER_MEDAL		10
#define GOLD_MEDAL			11
#define PIPE				12
#define TITLE				13
#define GAME_OVER			14						/* ��ͼƬ�������Ŷ�Ӧ, ��
													   �����漰һЩ����ͱ����
													   ��, ������������������
													   �ŵĶ�Ӧ��ϵ */

const int IMAGE_NUMBER = 15;						/* ͼƬ���� */
const char *IMAGE_PATH[IMAGE_NUMBER + 5] = {		/* ͼƬ�ļ�λ�� */
	"resource/background1.png",	// 0	���챳��
	"resource/background2.png",	// 1	ҹ����
	"resource/bird1.png",		// 2	�� (�������)
	"resource/bird2.png",		// 3	�� (���ˮƽ)
	"resource/bird3.png",		// 4	�� (�������)
	"resource/board.png",		// 5	�Ƿְ�
	"resource/button1.png",		// 6	��ť (δ����)
	"resource/button2.png",		// 7	��ť (����)
	"resource/ground.png",		// 8	����
	"resource/instruction.png",	// 9	��ʼ��ʾ
	"resource/medal1.png",		// 10	������
	"resource/medal2.png",		// 11	����
	"resource/pipe.png",		// 12	�ܵ� (����)
	"resource/title.png",		// 13	���� (Flappy Bird)
	"resource/gameOver.png",	// 14	Game Over
};
const int IMAGE_SIZE[IMAGE_NUMBER][2] = { 			/* ͼƬ�Ŀ�͸� */
	{ 285, 510 },
	{ 285, 510 },
	{ 35, 25 },
	{ 35, 25 },
	{ 35, 25 },
	{ 228, 116 },
	{ 116, 69 },
	{ 116, 69 },
	{ 285, 110 },
	{ 113, 100 },
	{ 45, 45 },
	{ 45, 45 },
	{ 54, 321 },
	{ 185, 50 },
	{ 194, 44 }
};
const int WINDOW_WIDTH = 285, WINDOW_HEIGHT = 510;	/* ���ڴ�С */
const int FPS = 60;									/* ֡��, ��Ϸʱ�䵥λΪ֡,
													   ���ȵ�λΪ���� */
const float GA = 0.42;								/* �������ٶ� (ÿ֡���ӵ���
													   ��) */
const float DOWN_SPEED = 8.2;						/* ��ͷ��ֱ����ʱ���ٶ�, ��
													   ��ת���ٶȺͷ��нǶ� */
const float CLICK_SPEED = -7.5;						/* һ�ε����������ٶ� */
const float SMOOTH_ROTATE_SPEED = 6;				/* Ϊ����ת��̫������������
													   ������, ʹ������ʱ���½�
													   ��ʼʱ�������ٶ���תͼƬ */
const float MAX_DOWN_SPEED = 8;						/* ���ǿ�������, ���������
													   �½��ٶ� */
const int TIME_PER_WINGING = 3;						/* �ȳ�� (�ı���̬) ��ʱ��
													   ��� */
const float PIPE_SPEED = -2;						/* �ܵ��ƶ��ٶ� */
const int CRASH_SIZE = -2;							/* ���ǵ��ܵ����񶼲��Ǿ���,
														ʵ����ײ�����ͼƬ��СС */
//const int PIPE_GAP_VERTICAL = 90;					/* ���¹ܵ���϶ (��̬) */
const int PIPE_GAP_VERTICAL = 105;					/* ���¹ܵ���϶ (����) */
//const int PIPE_GAP_VERTICAL = 150;				/* ���¹ܵ���϶ (�����޵�) */
const int PIPE_GAP_HORIZONTAL = 260;				/* ���ҹܵ���϶��׼ֵ */
const int PIPE_GAP_HORIZONTAL_RANGE = 100;			/* ���ҹܵ���϶�����Χ */
const int MAX_PIPE_NUMBER = 8;						/* ��Ļ�������ֶ��ٹܵ� */
const float INITIAL_SPEEDX = 2;						/* ��ʼʱ������Ļ���ٶ� */
const int CHANGE_BACKGROUND_TIME = 128;				/* �ı���ʱ�Ļ���ʱ�� */
const int SCORE_COLOR = WHITE;						/* ��Ϸ�з�����ɫ */
const char *SCORE_FONT = "Comic Sans MS";			/* ��Ϸ�з������� */
const int SCORE_HEIGHT = 40;						/* ��Ϸ�з�������߶� */
const int SCORE_WIDTH = 20;							/* ��Ϸ�з��������� */
const int SCORE_WEIGHT = FW_ULTRABOLD;				/* ��Ϸ�з��������ϸ */
const int SCORE_Y = 80;								/* ��Ϸ�з������� y ���� */
const int GAME_OVER_WAIT = 0;						/* ��ʾ Game Over ��ʱ�� */
const int TITLE_APPEAR_TIME = 80;					/* ������ʾ��������ʾ��ʱ�� */
const int BOARD_APPEAR_TIME = 80;					/* ������ʾ�Ʒְ��ʱ�� */
const char *END_SCORE_FONT = "Comic Sans MS";		/* ���յ÷����� */
const int END_SCORE_HEIGHT = 24;					/* ���յ÷�����߶� */
const int END_SCORE_WIDTH = 12;						/* ���յ÷������� */
const int END_SCORE_X = 230;						/* ���յ÷������Ҳ� x ���� (
													   ��Ϊ���Ҷ˶���) */
const int END_SCORE_Y = 184;						/* ���յ÷��������� y ���� */
const int END_MAX_SCORE_Y = 226;					/* ������ѵ÷��������� y ��
													   �� ( ��������ͬ )*/
const int END_SCORE_WEIGHT = FW_MEDIUM;				/* ���յ÷������ϸ */
const int MEDAL_X = 54;								/* �������Ͻ� x ���� */
const int MEDAL_Y = 183;							/* �������Ͻ� y ���� */
const char *MAX_SCORE_PATH = "maxScore.txt";		/* ��ѵ÷��ļ� */
const int SHINING_TIME = 90;						/* �Ƽ�¼ʱ�÷�����ʱ�� */
const int SHINING_TIMES = 10;						/* �Ƽ�¼ʱ�÷����Ĵ��� */
const int SHINING_SCORE_COLOR =
							EGERGB(255, 105, 40);	/* �Ƽ�¼ʱ��˸��ɫ */
const char *SHINING_SCORE_FONT = "Comic Sans MS";	/* �Ƽ�¼ʱ�������� */
const int SHINING_SCORE_HEIGHT = 48;				/* �Ƽ�¼ʱ��������߶� */
const int SHINING_SCORE_WIDTH = 24;					/* �Ƽ�¼ʱ���������� */
const int SHINING_SCORE_WEIGHT = FW_ULTRABOLD;		/* �Ƽ�¼ʱ���������ϸ */
const int BUTTON_X = 84;							/* ��ť���Ͻ� x ����*/
const int BUTTON_Y = 270;							/* ��ť���Ͻ� y ����*/

PIMAGE image[IMAGE_NUMBER + 5]; 					/* ��������ͼ�� */
int background;										/* ��ǰ����ͼƬ */

struct bird {										/* ��ṹ�� */
	int posX, posY;									/* ͼ�����Ͻ�λ�� */
	int shape,										/* ��̬ (2 / 3 / 4 <=> ���
													   ���� / ˮƽ / ����) */
		cnt;										/* ֡����, �� speed < 0 ��
													   cnt % TIME_PER_WINGING =
													   0 ʱ�л���̬; �� speed =
													   0 ʱΪ��̬ 3; �� speed <
													   0 ʱΪ��̬ 4 */
	float speedX, speedY;							/* �� x, y ������Ϊ��, ͼ��
													   �Ƕ����ٶ����, ���ڿ���
													   ����߽���, ������Ҫ x ��
													   ����ٶ� */
} player;

int pipeHead, pipeTail;								/* ʹ��ѭ�����д���ܵ�, ��
													   ��ʹ�� STL, ����ҿ� */
std::pair<int, int> pipeOnScreen[MAX_PIPE_NUMBER
									+ 5];			/* ���������¹ܵ������Ͻ� */
int nextPipeToMeet;									/* ��ǰ����ǰ���Ĺܵ��ڶ���
													   �е�λ�� */
int score;											/* �÷� */

int birdCrash(const int &x, const int &y);			/* �ж����Ͻ�Ϊ (x, y) ����
													   �Ƿ�����ײ, ���ж��Ƿ�
													   ��ǰ�ܵ� */
int drawNextBird(bird &cur);						/* ����һ֡�񲢽�����ײ���,
													   �����Ƿ�����ײ */
void drawNextPipe(const int &speed);				/* ����һ֡�ܵ����ں���ʱ��
													   �����¹ܵ� */
int getPipeHeight();								/* ����õ�һ���ܵ��ĸ߶� */
int circleNext(int pos);							/* ���� pos ģ
													   MAX_PIPE_NUMBER ѭ�� + 1
													   ���ֵ */
void reprintBackground();							/* ���´�ӡ���� */
void reprintGround();								/* ���´�ӡ���� (������һ��
													   ��, ����Ҫ��ʾ���ܵ���ǰ
													   ��, ��Ҫ��������һ������) */

int main() {
	setcaption("Flappy Bird");
	// ���ô��ڱ���

	setinitmode(INIT_WITHLOGO | INIT_NOFORCEEXIT, 100, 100);
	// ��ʾ���� LOGO | �رմ���ʱ��ǿ�ƽ�������
	// INIT_NOFORCEEXIT ��ζ���г�ʱ��ѭ��ʱ�����ж� is_run()
	// ���������û��޷��رմ��ڵ����

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	// ��ʼ������

	randomize();
	// ���������ʼ��

	for (int i = 0; i < IMAGE_NUMBER; i++) {
		image[i] = newimage();
		getimage(image[i], IMAGE_PATH[i]);
	}
	// ��ʼ��ͼƬ

	putimage(0, 0, image[BACKGROUND_DAY]);
	putimage(0, WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1], image[GROUND]);
	// ��ӡ����

//	getch();

	for (int i = 0; i <= TITLE_APPEAR_TIME && is_run(); i++) {
		reprintBackground();
		reprintGround();		// ע��Ҫ��ӡ���������������õ�͸���Ⱦ��Ǽٵ�
		putimage_alphatransparent(NULL, image[TITLE],
							WINDOW_WIDTH / 2 - IMAGE_SIZE[TITLE][0] / 2,
							WINDOW_HEIGHT / 2 - IMAGE_SIZE[TITLE][1] / 2 -
							IMAGE_SIZE[TITLE][1] / 2 - 100,
							0, i * 0xFF / TITLE_APPEAR_TIME);
		delay_fps(FPS);
	}
	delay_ms(1000);
	putimage_withalpha(NULL, image[INSTRUCTION],
						WINDOW_WIDTH / 2 - IMAGE_SIZE[INSTRUCTION][0] / 2,
						WINDOW_HEIGHT / 2 - IMAGE_SIZE[INSTRUCTION][1] / 2);
	// ��ӡ��ʾ�ͱ���

	flushmouse();
	// ��������Ϣ������

	mouse_msg msg = { 0 };

	bool isBegin = false;
	while (!isBegin && is_run()) {
		msg = getmouse();
		if (msg.is_left() && msg.is_down())
			isBegin = true;
	}
	// �ȴ���ʼ

	gameBegin:									// ��Ϸ��ʼ
	flushmouse();

	background = BACKGROUND_DAY;

	int maxScore = 0;
	std::ofstream outMaxScore(MAX_SCORE_PATH,
							std::ios::in);		// ����� (û�����½�, �����
												// ios::in ���������ļ�)
	std::ifstream inMaxScore(MAX_SCORE_PATH);	// ������
	if (inMaxScore.peek() != EOF)				// �м�¼���ȡ
		inMaxScore >> maxScore;
	// ��ȡ��ʷ������

	player.shape = 3;
	player.posX = 0;
	player.posY = (WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1]) / 2 -
					IMAGE_SIZE[player.shape][1] / 2;
	player.cnt = 0;
	player.speedX = INITIAL_SPEEDX;
	player.speedY = CLICK_SPEED;
	// ��ʼ����ҵ���

	pipeHead = pipeTail = nextPipeToMeet = 1;
	pipeOnScreen[pipeTail] = { WINDOW_WIDTH, getPipeHeight() };
	pipeTail = circleNext(pipeTail);
	// ��ʼ����һ���ܵ�

	setfont(SCORE_HEIGHT, SCORE_WIDTH, SCORE_FONT, 0, 0,
			SCORE_WEIGHT, false, false, false);
	setcolor(SCORE_COLOR);
	// ���÷��������弰��ɫ

	int lastScore = 0;
	int shineCount = -1;
	// ���Ƿ�ֹ��������

	score = 0;
	bool brokenRecord = false;

	while (is_run()) {
		reprintBackground();

		if (drawNextBird(player))					// ���˶����ж��Ƿ���ײ
			break;									// ��ײ����Ϸ����

		if (player.speedX == 0)						// �������˶��ٿ�ʼ����ܵ�
			drawNextPipe(PIPE_SPEED);				// �ܵ��˶�

		reprintGround();							// ȷ��������ʾ����ǰ

//		xyprintf(10, 10, "%d", score);
//		xyprintf(10, 10, "%d", maxScore);

		if (score == maxScore + 1) {
			if (shineCount == -1) {					// �Ƽ�¼����ֻ����һ��
				brokenRecord = true;
				shineCount = SHINING_TIME;
				setfont(SHINING_SCORE_HEIGHT, SHINING_SCORE_WIDTH,
						SHINING_SCORE_FONT, 0, 0,
						SHINING_SCORE_WEIGHT, false, false, false);
				// ������
			}
			maxScore = score;
			outMaxScore.seekp(std::ios::beg);
			outMaxScore << maxScore;
			// ʵʱ�����ļ��е�������
		}
		if (shineCount > 0) {
			if (shineCount % (SHINING_TIME / SHINING_TIMES) == 0) {
				int c = shineCount / (SHINING_TIME / SHINING_TIMES);
				if (c & 1) setcolor(SCORE_COLOR);			// �ָ���ɫ
				else setcolor(SHINING_SCORE_COLOR);			// ��ɫ
			}
			--shineCount;
			if (!shineCount) {
				setfont(SCORE_HEIGHT, SCORE_WIDTH, SCORE_FONT, 0, 0,
						SCORE_WEIGHT, false, false, false);
				setcolor(SCORE_COLOR);						// �����
			}
		}
		char scoreString[20];
		sprintf(scoreString, "%d", score);
		int len = strlen(scoreString);
		if (shineCount > 0)						// ��˸ʱ�����С��ͬλ��ҲҪ��
			ege_drawtext(scoreString,
						WINDOW_WIDTH / 2 - len * SHINING_SCORE_WIDTH / 2,
						SCORE_Y - SHINING_SCORE_HEIGHT / 2);
		else
			ege_drawtext(scoreString, WINDOW_WIDTH / 2 - len * SCORE_WIDTH / 2,
						SCORE_Y - SCORE_HEIGHT / 2);
		// ��ӡ����

		if (score != lastScore && score % 10 == 0) {	// lastScore ��ֹ����
			background = (background + 1) % 2,
			lastScore = score;							// �л�����
		}

//		xyprintf(10, 50, "%d %d %d", pipeHead, pipeTail, nextPipeToMeet);

		bool isClick = false;
		bool isPause = false;

		while (mousemsg()) {
			msg = getmouse();
			if (msg.is_left() && msg.is_down())				// �ж��Ƿ��������
				isClick = true;
			if (msg.is_right() && msg.is_down())			// �ж��Ƿ��Ҽ�����
				isPause = true;
		}

		while (isPause && is_run()) {
			while (mousemsg()) {
				msg = getmouse();
				if (msg.is_right() && msg.is_down())
					isPause = false;
				if (msg.is_left() && msg.is_down())
					isPause = false, isClick = true;
			}
			delay_fps(FPS);
		}
		// �Ҽ�����, ��ͣ; ������Ҽ����������ͣ

		if (isClick)
			player.speedY = CLICK_SPEED;
		// �������, �ٶȸı�

		delay_fps(FPS);										// �ӳ�
	}
	// ��Ϸ����

	bool atGround = false;
	while (!atGround && is_run()) {
		reprintBackground();
		drawNextPipe(0);
		if (drawNextBird(player) == 2)
			atGround = true;
		reprintGround();						// ȷ��������ʾ����ǰ
		delay_fps(FPS);
	}
	// ��ʾ��ײ���ܵ��������Ķ���

	putimage_withalpha(NULL, image[GAME_OVER],
						WINDOW_WIDTH / 2 - IMAGE_SIZE[GAME_OVER][0] / 2,
						WINDOW_HEIGHT / 2 - IMAGE_SIZE[GAME_OVER][1] / 2 - 160);
	delay_ms(GAME_OVER_WAIT * 1000 / FPS);
	// ��ӡ Game Over


	for (int i = 0; i <= BOARD_APPEAR_TIME && is_run(); i++) {
		reprintBackground();
		reprintGround();
		putimage_withalpha(NULL, image[GAME_OVER],
						WINDOW_WIDTH / 2 - IMAGE_SIZE[GAME_OVER][0] / 2,
						WINDOW_HEIGHT / 2 - IMAGE_SIZE[GAME_OVER][1] / 2 - 160);
		// ע��Ҫ��ӡ���������������õ�͸���Ⱦ��Ǽٵ�
		putimage_alphatransparent(NULL, image[BOARD],
							WINDOW_WIDTH / 2 - IMAGE_SIZE[BOARD][0] / 2,
							WINDOW_HEIGHT / 2 - IMAGE_SIZE[BOARD][0] / 2,
							EGERGB(255, 255, 255),			// ���ð�ɫΪ͸��ɫ
							i * 0xFF / BOARD_APPEAR_TIME);
		delay_fps(FPS);
	}

	flushmouse();

	gameOver:
	reprintBackground();
	reprintGround();
	putimage_withalpha(NULL, image[GAME_OVER],
					WINDOW_WIDTH / 2 - IMAGE_SIZE[GAME_OVER][0] / 2,
					WINDOW_HEIGHT / 2 - IMAGE_SIZE[GAME_OVER][1] / 2 - 160);
	// ��ӡ����
	putimage_alphatransparent(NULL, image[BOARD],
							WINDOW_WIDTH / 2 - IMAGE_SIZE[BOARD][0] / 2,
							WINDOW_HEIGHT / 2 - IMAGE_SIZE[BOARD][0] / 2,
							EGERGB(255, 255, 255),			// ���ð�ɫΪ͸��ɫ
							0xFF);
	// ��ӡ�Ʒְ�


	char scoreString[20];
	sprintf(scoreString, "%d", score);
	int len = strlen(scoreString);
	setfont(END_SCORE_HEIGHT, END_SCORE_WIDTH, END_SCORE_FONT, 0, 0,
			END_SCORE_WEIGHT, false, false, false);
	setcolor(brokenRecord ? SHINING_SCORE_COLOR : SCORE_COLOR);
	// ���Ƽ�¼����ɫ
	ege_drawtext(scoreString, END_SCORE_X - len * END_SCORE_WIDTH, // �Ҷ���
				END_SCORE_Y - END_SCORE_HEIGHT / 2);
	// ��ӡ�÷�
	sprintf(scoreString, "%d", maxScore);
	len = strlen(scoreString);
	ege_drawtext(scoreString, END_SCORE_X - len * END_SCORE_WIDTH, // �Ҷ���
				END_MAX_SCORE_Y - END_SCORE_HEIGHT / 2);
	// ��ӡ��ѵ÷�

	int medal = score >= 30 ? GOLD_MEDAL : SILVER_MEDAL;
	if (score >= 10)
		putimage_withalpha(NULL, image[medal], MEDAL_X, MEDAL_Y);
	// ��ӡ����

//	msg = getmouse();

//	getch();

	if (is_run()) {
		putimage_withalpha(NULL, image[BUTTON_LIGHT], BUTTON_X, BUTTON_Y);

		int x, y;
		mousepos(&x, &y);
//		xyprintf(10, 10, "%d %d", x, y);
		if (x >= BUTTON_X && x <= BUTTON_X + IMAGE_SIZE[BUTTON_LIGHT][0] &&
			y >= BUTTON_Y && y <= BUTTON_Y + IMAGE_SIZE[BUTTON_LIGHT][1]) {
			putimage_withalpha(NULL, image[BUTTON_DARK], BUTTON_X, BUTTON_Y);
			// ��ť�䰵����������
			bool isClick = false;
			while (mousemsg()) {
				msg = getmouse();
				if (msg.is_left() && msg.is_down())		// �ж��Ƿ��������
					isClick = true;
			}
			if (isClick)
				goto gameBegin;							// ���¿�ʼ
		}

		delay_fps(FPS);
		goto gameOver;
		// ���ڰ�ť����Ӱ�ظ���ӡ����, ������ѭ���Ļ�Ҫ��ӡ�ܶ�ͼƬ
		// ����ֱ������ goto
	}

	for (int i = 0; i < IMAGE_NUMBER; i++)
		delimage(image[i]);
	// ����ͼ��, �ͷ��ڴ�

	return 0;
}

inline int birdCrash(bird cur) {
	int x = cur.posX, y = cur.posY;
	int w = IMAGE_SIZE[BIRD_UP][0],
		h = IMAGE_SIZE[BIRD_UP][1];
	int lft = x - CRASH_SIZE, rgt = x + w + CRASH_SIZE,
		up = y - CRASH_SIZE, dwn = y + h + CRASH_SIZE;
	// ʵ����ײ�����ͼƬ��СС
	int groundHeight = IMAGE_SIZE[GROUND][1];
	static bool added = false;
	if (cur.speedX) added = false; 					// ��Ϸ��ʼʱ�ĳ�ʼ��
	if (y + h >= WINDOW_HEIGHT - groundHeight)		// ���
		return 2;
//	xyprintf(10, 80, "%d", nextPipeToMeet);
	if ((nextPipeToMeet >= pipeHead && nextPipeToMeet < pipeTail) ||
		(nextPipeToMeet < pipeTail && pipeTail <= pipeHead) ||
		(nextPipeToMeet >= pipeHead && pipeHead >= pipeTail)) {
													// ��ǰ�йܵ� (����ѭ������)
		int pipeX = pipeOnScreen[nextPipeToMeet].first,
			pipeY = pipeOnScreen[nextPipeToMeet].second;
		if (x + IMAGE_SIZE[BIRD_UP][0] / 2 >= pipeX && !added)
													// ��һ��ͨ����ǰ�ܵ�
			++score, added = true;					// �ӷ�
		if (lft > pipeX + IMAGE_SIZE[PIPE][0]) {	// ��ȫͨ����ǰ�ܵ�
			nextPipeToMeet = circleNext(nextPipeToMeet);
			added = false;
		}
//		xyprintf(30, 10, "%d %d", y, pipeY);
		return rgt >= pipeX && lft <= pipeX + IMAGE_SIZE[PIPE][0] &&
			(!(up > pipeY - PIPE_GAP_VERTICAL && dwn < pipeY));
	}
	return 0;
}

inline float speedToAngle(const float &speed) {
	if (speed < SMOOTH_ROTATE_SPEED) return PI / 6;
	// ����ʱ�͸տ�ʼ�½�ʱ�ȶ�Ϊ���� 30 ��
	return -((speed - SMOOTH_ROTATE_SPEED) /
			(DOWN_SPEED - SMOOTH_ROTATE_SPEED) * PI / 2) + PI / 6;
	// ����ʱ�����ٶ�ռ��ֱ�����ٶȵı���, ת��Ϊ�Ƕ�, ���ڸտ�ʼ�½�ʱû����ת,
	// Ҫ�����ǲ����ٶȲ���ƽ����ת
}

inline void reprintBackground() {
	static int lastBackground = background, cnt = 0; // cnt ��Ӧ change ���ʱ��
	if (lastBackground != background && !cnt)
		cnt = CHANGE_BACKGROUND_TIME;
	putimage(0, 0, image[lastBackground]);
	// ע������� lastbackground �Դﵽ������Ŀ��
	if (cnt) {	// �ٻ���±���
		putimage_alphablend(NULL, image[background], 0, 0,
							(CHANGE_BACKGROUND_TIME - cnt) * 0xFF /
							CHANGE_BACKGROUND_TIME); // ���� cnt ����͸����
		if (!(--cnt)) lastBackground = background;
	}
}

inline void reprintGround() {
	putimage(0, WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1], image[GROUND]);
}

inline int drawNextBird(bird &p) {
	int w = IMAGE_SIZE[p.shape][0], h = IMAGE_SIZE[p.shape][1];
	p.cnt = (p.cnt + 1) % TIME_PER_WINGING;
	if (p.speedY < 0 && p.cnt == 0)
		p.shape = (p.shape - 1) % 3 + 2;	// �л������̬
	if (p.speedY < MAX_DOWN_SPEED)			// �޶�����½��ٶ�
		p.speedY += GA;
	float angle = speedToAngle(p.speedY);	// �����ٶȼ���ͼ��Ҫƫת�ĽǶ�
	p.posY += (int)p.speedY;
	p.posX += (int)p.speedX;
	if (p.posX >= WINDOW_WIDTH / 2 - IMAGE_SIZE[BIRD_UP][0] / 2)
	// ��ʼһ��ʱ������˶�
		p.posX = WINDOW_WIDTH / 2 - IMAGE_SIZE[BIRD_UP][0] / 2 - 1,
		p.speedX = 0;
	if (p.posY < 0) p.posY = 0;				// �������ϳ���, ��������������ʧ��
	w /= 2, h /= 2;
	putimage_rotate(NULL, image[p.shape], p.posX + w, p.posY + h, 0.5, 0.5,
					angle, 1);				// ��תһ���Ƕ����
//	xyprintf(100, 10, "111");
	return birdCrash(player);				// ��ײ�ж�
}

void drawNextPipe(const int &speed) {
	int d = random(2 * PIPE_GAP_HORIZONTAL_RANGE) - PIPE_GAP_HORIZONTAL_RANGE +
			PIPE_GAP_HORIZONTAL;		// ������һ���ܵ����¼ӹܵ�֮��ļ��
	for (int i = pipeHead; i != pipeTail; i = circleNext(i)) {
		std::pair<int, int> &p = pipeOnScreen[i];
		int x = p.first, y = p.second;
		putimage_withalpha(NULL, image[PIPE], x, y);			// ����°벿��
		putimage_rotate(NULL, image[PIPE], x + IMAGE_SIZE[PIPE][0] / 2,
						y - PIPE_GAP_VERTICAL, 0.5, 0, PI, 1);	// ����ϰ벿��
		p.first += (int)speed;									// �ƶ�
		if (i == pipeHead) {
			if (p.first + IMAGE_SIZE[PIPE][0] < 0)		// ����ߵĹܵ�������
				pipeHead = circleNext(pipeHead);
		}
		if (circleNext(i) == pipeTail) {
			if (p.first + d <= WINDOW_WIDTH) {			// �����¼�һ���ܵ�
				pipeOnScreen[pipeTail] = { WINDOW_WIDTH, getPipeHeight() };
														// ȷ���Ӵ������ұ߽���
				pipeTail = circleNext(pipeTail);
			}
		}
	}
}

inline int getPipeHeight() {
	return random(WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1] - PIPE_GAP_VERTICAL
			- 40) + PIPE_GAP_VERTICAL + 40;	// �����ϲ����²�����һ���ļ��
}

inline int circleNext(int pos) {
	return pos == MAX_PIPE_NUMBER ? 1 : (pos + 1);
}
