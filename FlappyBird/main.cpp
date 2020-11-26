#include <graphics.h>
#include <bits/stdc++.h>

#define delay_ms(t) \
[](const int &msTime) { \
	int fpsTime = msTime * FPS / 1000; \
	while (fpsTime-- && is_run()) \
		delay_fps(FPS); \
} (t)
// 将 delay_ms 转化为 delay_fps (因为要判断 is_run 所以不能直接 delay_ms)
// 用 lambda 可以避免变量重名

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
#define GAME_OVER			14						/* 将图片名称与标号对应, 代
													   码中涉及一些运算和标号有
													   关, 不可随意更改名称与标
													   号的对应关系 */

const int IMAGE_NUMBER = 15;						/* 图片数量 */
const char *IMAGE_PATH[IMAGE_NUMBER + 5] = {		/* 图片文件位置 */
	"resource/background1.png",	// 0	白天背景
	"resource/background2.png",	// 1	夜晚背景
	"resource/bird1.png",		// 2	鸟 (翅膀向上)
	"resource/bird2.png",		// 3	鸟 (翅膀水平)
	"resource/bird3.png",		// 4	鸟 (翅膀向下)
	"resource/board.png",		// 5	记分板
	"resource/button1.png",		// 6	按钮 (未激活)
	"resource/button2.png",		// 7	按钮 (激活)
	"resource/ground.png",		// 8	地面
	"resource/instruction.png",	// 9	开始提示
	"resource/medal1.png",		// 10	银奖章
	"resource/medal2.png",		// 11	金奖章
	"resource/pipe.png",		// 12	管道 (朝上)
	"resource/title.png",		// 13	标题 (Flappy Bird)
	"resource/gameOver.png",	// 14	Game Over
};
const int IMAGE_SIZE[IMAGE_NUMBER][2] = { 			/* 图片的宽和高 */
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
const int WINDOW_WIDTH = 285, WINDOW_HEIGHT = 510;	/* 窗口大小 */
const int FPS = 60;									/* 帧率, 游戏时间单位为帧,
													   长度单位为像素 */
const float GA = 0.42;								/* 重力加速度 (每帧增加的速
													   度) */
const float DOWN_SPEED = 8.2;						/* 鸟头竖直朝下时的速度, 用
													   于转换速度和飞行角度 */
const float CLICK_SPEED = -7.5;						/* 一次点击赋予鸟的速度 */
const float SMOOTH_ROTATE_SPEED = 6;				/* 为了旋转不太剧烈设置了这
													   个参数, 使得上升时和下降
													   开始时不按照速度旋转图片 */
const float MAX_DOWN_SPEED = 8;						/* 考虑空气阻力, 设置了最大
													   下降速度 */
const int TIME_PER_WINGING = 3;						/* 扇翅膀 (改变形态) 的时间
													   间隔 */
const float PIPE_SPEED = -2;						/* 管道移动速度 */
const int CRASH_SIZE = -2;							/* 考虑到管道和鸟都不是矩形,
														实际碰撞体积比图片大小小 */
//const int PIPE_GAP_VERTICAL = 90;					/* 上下管道间隙 (变态) */
const int PIPE_GAP_VERTICAL = 105;					/* 上下管道间隙 (正常) */
//const int PIPE_GAP_VERTICAL = 150;				/* 上下管道间隙 (几乎无敌) */
const int PIPE_GAP_HORIZONTAL = 260;				/* 左右管道间隙基准值 */
const int PIPE_GAP_HORIZONTAL_RANGE = 100;			/* 左右管道间隙随机范围 */
const int MAX_PIPE_NUMBER = 8;						/* 屏幕上最多出现多少管道 */
const float INITIAL_SPEEDX = 2;						/* 鸟开始时进入屏幕的速度 */
const int CHANGE_BACKGROUND_TIME = 128;				/* 改背景时的缓冲时间 */
const int SCORE_COLOR = WHITE;						/* 游戏中分数颜色 */
const char *SCORE_FONT = "Comic Sans MS";			/* 游戏中分数字体 */
const int SCORE_HEIGHT = 40;						/* 游戏中分数字体高度 */
const int SCORE_WIDTH = 20;							/* 游戏中分数字体宽度 */
const int SCORE_WEIGHT = FW_ULTRABOLD;				/* 游戏中分数字体粗细 */
const int SCORE_Y = 80;								/* 游戏中分数中心 y 坐标 */
const int GAME_OVER_WAIT = 0;						/* 显示 Game Over 的时间 */
const int TITLE_APPEAR_TIME = 80;					/* 渐变显示标题与提示的时间 */
const int BOARD_APPEAR_TIME = 80;					/* 渐变显示计分板的时间 */
const char *END_SCORE_FONT = "Comic Sans MS";		/* 最终得分字体 */
const int END_SCORE_HEIGHT = 24;					/* 最终得分字体高度 */
const int END_SCORE_WIDTH = 12;						/* 最终得分字体宽度 */
const int END_SCORE_X = 230;						/* 最终得分字体右侧 x 坐标 (
													   因为是右端对齐) */
const int END_SCORE_Y = 184;						/* 最终得分字体中心 y 坐标 */
const int END_MAX_SCORE_Y = 226;					/* 最终最佳得分字体中心 y 坐
													   标 ( 其余与上同 )*/
const int END_SCORE_WEIGHT = FW_MEDIUM;				/* 最终得分字体粗细 */
const int MEDAL_X = 54;								/* 奖章左上角 x 坐标 */
const int MEDAL_Y = 183;							/* 奖章左上角 y 坐标 */
const char *MAX_SCORE_PATH = "maxScore.txt";		/* 最佳得分文件 */
const int SHINING_TIME = 90;						/* 破纪录时得分闪的时间 */
const int SHINING_TIMES = 10;						/* 破纪录时得分闪的次数 */
const int SHINING_SCORE_COLOR =
							EGERGB(255, 105, 40);	/* 破纪录时闪烁颜色 */
const char *SHINING_SCORE_FONT = "Comic Sans MS";	/* 破纪录时分数字体 */
const int SHINING_SCORE_HEIGHT = 48;				/* 破纪录时分数字体高度 */
const int SHINING_SCORE_WIDTH = 24;					/* 破纪录时分数字体宽度 */
const int SHINING_SCORE_WEIGHT = FW_ULTRABOLD;		/* 破纪录时分数字体粗细 */
const int BUTTON_X = 84;							/* 按钮左上角 x 坐标*/
const int BUTTON_Y = 270;							/* 按钮左上角 y 坐标*/

PIMAGE image[IMAGE_NUMBER + 5]; 					/* 储存所有图像 */
int background;										/* 当前背景图片 */

struct bird {										/* 鸟结构体 */
	int posX, posY;									/* 图像左上角位置 */
	int shape,										/* 形态 (2 / 3 / 4 <=> 翅膀
													   向上 / 水平 / 向下) */
		cnt;										/* 帧计数, 当 speed < 0 且
													   cnt % TIME_PER_WINGING =
													   0 时切换形态; 当 speed =
													   0 时为形态 3; 当 speed <
													   0 时为形态 4 */
	float speedX, speedY;							/* 以 x, y 正方向为正, 图像
													   角度与速度相关, 由于开场
													   从左边进入, 所以需要 x 方
													   向的速度 */
} player;

int pipeHead, pipeTail;								/* 使用循环队列储存管道, 不
													   能使用 STL, 左闭右开 */
std::pair<int, int> pipeOnScreen[MAX_PIPE_NUMBER
									+ 5];			/* 储存所有下管道的左上角 */
int nextPipeToMeet;									/* 当前鸟正前方的管道在队列
													   中的位置 */
int score;											/* 得分 */

int birdCrash(const int &x, const int &y);			/* 判断左上角为 (x, y) 的鸟
													   是否发生碰撞, 并判断是否
													   当前管道 */
int drawNextBird(bird &cur);						/* 画下一帧鸟并进行碰撞检测,
													   返回是否发生碰撞 */
void drawNextPipe(const int &speed);				/* 画下一帧管道并在合适时刻
													   出现新管道 */
int getPipeHeight();								/* 随机得到一个管道的高度 */
int circleNext(int pos);							/* 返回 pos 模
													   MAX_PIPE_NUMBER 循环 + 1
													   后的值 */
void reprintBackground();							/* 重新打印背景 */
void reprintGround();								/* 重新打印地面 (背景的一部
													   分, 由于要显示到管道的前
													   面, 需要单独设置一个函数) */

int main() {
	setcaption("Flappy Bird");
	// 设置窗口标题

	setinitmode(INIT_WITHLOGO | INIT_NOFORCEEXIT, 100, 100);
	// 显示开场 LOGO | 关闭窗口时不强制结束程序
	// INIT_NOFORCEEXIT 意味着有长时间循环时必须判断 is_run()
	// 否则会出现用户无法关闭窗口的情况

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	// 初始化画布

	randomize();
	// 随机函数初始化

	for (int i = 0; i < IMAGE_NUMBER; i++) {
		image[i] = newimage();
		getimage(image[i], IMAGE_PATH[i]);
	}
	// 初始化图片

	putimage(0, 0, image[BACKGROUND_DAY]);
	putimage(0, WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1], image[GROUND]);
	// 打印背景

//	getch();

	for (int i = 0; i <= TITLE_APPEAR_TIME && is_run(); i++) {
		reprintBackground();
		reprintGround();		// 注意要重印背景否则下面设置的透明度就是假的
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
	// 打印提示和标题

	flushmouse();
	// 清空鼠标消息缓冲区

	mouse_msg msg = { 0 };

	bool isBegin = false;
	while (!isBegin && is_run()) {
		msg = getmouse();
		if (msg.is_left() && msg.is_down())
			isBegin = true;
	}
	// 等待开始

	gameBegin:									// 游戏开始
	flushmouse();

	background = BACKGROUND_DAY;

	int maxScore = 0;
	std::ofstream outMaxScore(MAX_SCORE_PATH,
							std::ios::in);		// 输出流 (没有则新建, 必须加
												// ios::in 否则会清空文件)
	std::ifstream inMaxScore(MAX_SCORE_PATH);	// 输入流
	if (inMaxScore.peek() != EOF)				// 有记录则读取
		inMaxScore >> maxScore;
	// 读取历史最大分数

	player.shape = 3;
	player.posX = 0;
	player.posY = (WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1]) / 2 -
					IMAGE_SIZE[player.shape][1] / 2;
	player.cnt = 0;
	player.speedX = INITIAL_SPEEDX;
	player.speedY = CLICK_SPEED;
	// 初始化玩家的鸟

	pipeHead = pipeTail = nextPipeToMeet = 1;
	pipeOnScreen[pipeTail] = { WINDOW_WIDTH, getPipeHeight() };
	pipeTail = circleNext(pipeTail);
	// 初始化第一个管道

	setfont(SCORE_HEIGHT, SCORE_WIDTH, SCORE_FONT, 0, 0,
			SCORE_WEIGHT, false, false, false);
	setcolor(SCORE_COLOR);
	// 设置分数的字体及颜色

	int lastScore = 0;
	int shineCount = -1;
	// 都是防止动画永动

	score = 0;
	bool brokenRecord = false;

	while (is_run()) {
		reprintBackground();

		if (drawNextBird(player))					// 鸟运动并判断是否碰撞
			break;									// 碰撞则游戏结束

		if (player.speedX == 0)						// 不横向运动再开始输出管道
			drawNextPipe(PIPE_SPEED);				// 管道运动

		reprintGround();							// 确保地面显示在最前

//		xyprintf(10, 10, "%d", score);
//		xyprintf(10, 10, "%d", maxScore);

		if (score == maxScore + 1) {
			if (shineCount == -1) {					// 破纪录动画只播放一次
				brokenRecord = true;
				shineCount = SHINING_TIME;
				setfont(SHINING_SCORE_HEIGHT, SHINING_SCORE_WIDTH,
						SHINING_SCORE_FONT, 0, 0,
						SHINING_SCORE_WEIGHT, false, false, false);
				// 换字体
			}
			maxScore = score;
			outMaxScore.seekp(std::ios::beg);
			outMaxScore << maxScore;
			// 实时更新文件中的最大分数
		}
		if (shineCount > 0) {
			if (shineCount % (SHINING_TIME / SHINING_TIMES) == 0) {
				int c = shineCount / (SHINING_TIME / SHINING_TIMES);
				if (c & 1) setcolor(SCORE_COLOR);			// 恢复颜色
				else setcolor(SHINING_SCORE_COLOR);			// 变色
			}
			--shineCount;
			if (!shineCount) {
				setfont(SCORE_HEIGHT, SCORE_WIDTH, SCORE_FONT, 0, 0,
						SCORE_WEIGHT, false, false, false);
				setcolor(SCORE_COLOR);						// 变回来
			}
		}
		char scoreString[20];
		sprintf(scoreString, "%d", score);
		int len = strlen(scoreString);
		if (shineCount > 0)						// 闪烁时字体大小不同位置也要变
			ege_drawtext(scoreString,
						WINDOW_WIDTH / 2 - len * SHINING_SCORE_WIDTH / 2,
						SCORE_Y - SHINING_SCORE_HEIGHT / 2);
		else
			ege_drawtext(scoreString, WINDOW_WIDTH / 2 - len * SCORE_WIDTH / 2,
						SCORE_Y - SCORE_HEIGHT / 2);
		// 打印分数

		if (score != lastScore && score % 10 == 0) {	// lastScore 防止永动
			background = (background + 1) % 2,
			lastScore = score;							// 切换背景
		}

//		xyprintf(10, 50, "%d %d %d", pipeHead, pipeTail, nextPipeToMeet);

		bool isClick = false;
		bool isPause = false;

		while (mousemsg()) {
			msg = getmouse();
			if (msg.is_left() && msg.is_down())				// 判断是否左键单击
				isClick = true;
			if (msg.is_right() && msg.is_down())			// 判断是否右键单击
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
		// 右键单击, 暂停; 左键或右键单击解除暂停

		if (isClick)
			player.speedY = CLICK_SPEED;
		// 左键单击, 速度改变

		delay_fps(FPS);										// 延迟
	}
	// 游戏主体

	bool atGround = false;
	while (!atGround && is_run()) {
		reprintBackground();
		drawNextPipe(0);
		if (drawNextBird(player) == 2)
			atGround = true;
		reprintGround();						// 确保地面显示在最前
		delay_fps(FPS);
	}
	// 显示鸟撞到管道过后掉落的动画

	putimage_withalpha(NULL, image[GAME_OVER],
						WINDOW_WIDTH / 2 - IMAGE_SIZE[GAME_OVER][0] / 2,
						WINDOW_HEIGHT / 2 - IMAGE_SIZE[GAME_OVER][1] / 2 - 160);
	delay_ms(GAME_OVER_WAIT * 1000 / FPS);
	// 打印 Game Over


	for (int i = 0; i <= BOARD_APPEAR_TIME && is_run(); i++) {
		reprintBackground();
		reprintGround();
		putimage_withalpha(NULL, image[GAME_OVER],
						WINDOW_WIDTH / 2 - IMAGE_SIZE[GAME_OVER][0] / 2,
						WINDOW_HEIGHT / 2 - IMAGE_SIZE[GAME_OVER][1] / 2 - 160);
		// 注意要重印背景否则下面设置的透明度就是假的
		putimage_alphatransparent(NULL, image[BOARD],
							WINDOW_WIDTH / 2 - IMAGE_SIZE[BOARD][0] / 2,
							WINDOW_HEIGHT / 2 - IMAGE_SIZE[BOARD][0] / 2,
							EGERGB(255, 255, 255),			// 设置白色为透明色
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
	// 重印背景
	putimage_alphatransparent(NULL, image[BOARD],
							WINDOW_WIDTH / 2 - IMAGE_SIZE[BOARD][0] / 2,
							WINDOW_HEIGHT / 2 - IMAGE_SIZE[BOARD][0] / 2,
							EGERGB(255, 255, 255),			// 设置白色为透明色
							0xFF);
	// 重印计分板


	char scoreString[20];
	sprintf(scoreString, "%d", score);
	int len = strlen(scoreString);
	setfont(END_SCORE_HEIGHT, END_SCORE_WIDTH, END_SCORE_FONT, 0, 0,
			END_SCORE_WEIGHT, false, false, false);
	setcolor(brokenRecord ? SHINING_SCORE_COLOR : SCORE_COLOR);
	// 打破纪录换颜色
	ege_drawtext(scoreString, END_SCORE_X - len * END_SCORE_WIDTH, // 右对齐
				END_SCORE_Y - END_SCORE_HEIGHT / 2);
	// 打印得分
	sprintf(scoreString, "%d", maxScore);
	len = strlen(scoreString);
	ege_drawtext(scoreString, END_SCORE_X - len * END_SCORE_WIDTH, // 右对齐
				END_MAX_SCORE_Y - END_SCORE_HEIGHT / 2);
	// 打印最佳得分

	int medal = score >= 30 ? GOLD_MEDAL : SILVER_MEDAL;
	if (score >= 10)
		putimage_withalpha(NULL, image[medal], MEDAL_X, MEDAL_Y);
	// 打印奖章

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
			// 按钮变暗产生互动感
			bool isClick = false;
			while (mousemsg()) {
				msg = getmouse();
				if (msg.is_left() && msg.is_down())		// 判断是否左键单击
					isClick = true;
			}
			if (isClick)
				goto gameBegin;							// 重新开始
		}

		delay_fps(FPS);
		goto gameOver;
		// 由于按钮的阴影重复打印会变黑, 这里用循环的话要重印很多图片
		// 所以直接用了 goto
	}

	for (int i = 0; i < IMAGE_NUMBER; i++)
		delimage(image[i]);
	// 销毁图像, 释放内存

	return 0;
}

inline int birdCrash(bird cur) {
	int x = cur.posX, y = cur.posY;
	int w = IMAGE_SIZE[BIRD_UP][0],
		h = IMAGE_SIZE[BIRD_UP][1];
	int lft = x - CRASH_SIZE, rgt = x + w + CRASH_SIZE,
		up = y - CRASH_SIZE, dwn = y + h + CRASH_SIZE;
	// 实际碰撞体积比图片大小小
	int groundHeight = IMAGE_SIZE[GROUND][1];
	static bool added = false;
	if (cur.speedX) added = false; 					// 游戏开始时的初始化
	if (y + h >= WINDOW_HEIGHT - groundHeight)		// 落地
		return 2;
//	xyprintf(10, 80, "%d", nextPipeToMeet);
	if ((nextPipeToMeet >= pipeHead && nextPipeToMeet < pipeTail) ||
		(nextPipeToMeet < pipeTail && pipeTail <= pipeHead) ||
		(nextPipeToMeet >= pipeHead && pipeHead >= pipeTail)) {
													// 当前有管道 (考虑循环队列)
		int pipeX = pipeOnScreen[nextPipeToMeet].first,
			pipeY = pipeOnScreen[nextPipeToMeet].second;
		if (x + IMAGE_SIZE[BIRD_UP][0] / 2 >= pipeX && !added)
													// 第一次通过当前管道
			++score, added = true;					// 加分
		if (lft > pipeX + IMAGE_SIZE[PIPE][0]) {	// 完全通过当前管道
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
	// 上升时和刚开始下降时稳定为向上 30 度
	return -((speed - SMOOTH_ROTATE_SPEED) /
			(DOWN_SPEED - SMOOTH_ROTATE_SPEED) * PI / 2) + PI / 6;
	// 下落时计算速度占竖直朝下速度的比例, 转化为角度, 由于刚开始下降时没有旋转,
	// 要减掉那部分速度才能平滑旋转
}

inline void reprintBackground() {
	static int lastBackground = background, cnt = 0; // cnt 对应 change 后的时间
	if (lastBackground != background && !cnt)
		cnt = CHANGE_BACKGROUND_TIME;
	putimage(0, 0, image[lastBackground]);
	// 注意先输出 lastbackground 以达到清屏的目的
	if (cnt) {	// 再混合新背景
		putimage_alphablend(NULL, image[background], 0, 0,
							(CHANGE_BACKGROUND_TIME - cnt) * 0xFF /
							CHANGE_BACKGROUND_TIME); // 根据 cnt 计算透明度
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
		p.shape = (p.shape - 1) % 3 + 2;	// 切换翅膀形态
	if (p.speedY < MAX_DOWN_SPEED)			// 限定最大下降速度
		p.speedY += GA;
	float angle = speedToAngle(p.speedY);	// 根据速度计算图像要偏转的角度
	p.posY += (int)p.speedY;
	p.posX += (int)p.speedX;
	if (p.posX >= WINDOW_WIDTH / 2 - IMAGE_SIZE[BIRD_UP][0] / 2)
	// 开始一段时间横向运动
		p.posX = WINDOW_WIDTH / 2 - IMAGE_SIZE[BIRD_UP][0] / 2 - 1,
		p.speedX = 0;
	if (p.posY < 0) p.posY = 0;				// 不能向上出界, 但碰到顶部不算失败
	w /= 2, h /= 2;
	putimage_rotate(NULL, image[p.shape], p.posX + w, p.posY + h, 0.5, 0.5,
					angle, 1);				// 旋转一定角度输出
//	xyprintf(100, 10, "111");
	return birdCrash(player);				// 碰撞判断
}

void drawNextPipe(const int &speed) {
	int d = random(2 * PIPE_GAP_HORIZONTAL_RANGE) - PIPE_GAP_HORIZONTAL_RANGE +
			PIPE_GAP_HORIZONTAL;		// 随机最后一个管道和新加管道之间的间隔
	for (int i = pipeHead; i != pipeTail; i = circleNext(i)) {
		std::pair<int, int> &p = pipeOnScreen[i];
		int x = p.first, y = p.second;
		putimage_withalpha(NULL, image[PIPE], x, y);			// 输出下半部分
		putimage_rotate(NULL, image[PIPE], x + IMAGE_SIZE[PIPE][0] / 2,
						y - PIPE_GAP_VERTICAL, 0.5, 0, PI, 1);	// 输出上半部分
		p.first += (int)speed;									// 移动
		if (i == pipeHead) {
			if (p.first + IMAGE_SIZE[PIPE][0] < 0)		// 最左边的管道出界了
				pipeHead = circleNext(pipeHead);
		}
		if (circleNext(i) == pipeTail) {
			if (p.first + d <= WINDOW_WIDTH) {			// 可以新加一个管道
				pipeOnScreen[pipeTail] = { WINDOW_WIDTH, getPipeHeight() };
														// 确保从窗口最右边进入
				pipeTail = circleNext(pipeTail);
			}
		}
	}
}

inline int getPipeHeight() {
	return random(WINDOW_HEIGHT - IMAGE_SIZE[GROUND][1] - PIPE_GAP_VERTICAL
			- 40) + PIPE_GAP_VERTICAL + 40;	// 距离上部和下部都有一定的间隔
}

inline int circleNext(int pos) {
	return pos == MAX_PIPE_NUMBER ? 1 : (pos + 1);
}
