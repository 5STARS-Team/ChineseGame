#ifndef _CLIENTDEFINE_H_ 
#define _CLIENTDEFINE_H_
#include <algorithm>
#include <string>
#include <map>

//namespace std;


/* ������Կ */
const std::string SERVER_KEY = "l$`h.V,&";


/*LAYER tag ֵ*/
const int MAIN_LAYER = 1000;
const int DOWNLOAD_LAYER = 1001;
const int LOGIN_LAYER = 1002;
const int ABOUT_LAYER = 1003;
const int ANSWER_LAYER = 1004;
const int DAILY_REWARD_LAYER = 1005;
const int HONOUR_LAYER = 1006;
const int ACTIVITY_LAYER = 1007;
const int PAY_LAYER = 1008;
/* scene tag*/
const int MAIN_SCENE = 100;


/* CODE*/
const int CODE_REG_RE = 1001; //ע��
const int CODE_REG_RS = 1002; //ע�᷵��

const int CODE_LOGIN_RE = 1003; //��¼
const int CODE_LOGIN_RS = 1004; //��¼����
const int CODE_LOGIN_FROM_CENTER_RE = 1005; //Զ�̵�¼

const int CODE_GAME_GET_COIN_RE = 1110;//������
const int CODE_GAME_GET_COIN_RS = 1111;//�����ҷ���
const int MSG_BET_SAMPLE_REQ = 1116;//������ע����
const int MSG_BET_SAMPLE_RSP = 1117;//������ע���󷵻�
const int MSG_ODDS_SAMPLE_REQ = 1118;//��ͨ������������
const int MSG_ODDS_SAMPLE_RSP = 1119;//��ͨ�����������ʷ���
const int MSG_FRUIYS_DA_XIA_REQ = 1120;//ˮ�����Ĵ�С
const int MSG_FRUIYS_DA_XIA_RSP = 1121;//ˮ�����Ĵ�С����






//music
const std::string BG_MUSIC = "mp3/bgm.mp3";
const std::string BUTTON_EFFECT = "mp3/button.mp3";
const std::string ANSWER_EFFECT = "mp3/click.mp3";
const std::string WIN_EFFECT = "mp3/pass.mp3";
const std::string FAILURE_EFFECT = "mp3/error.mp3";

//ÿ�ս���
const int dailyCoin[7]={100,150,150,200,200,300,800};
const int passCoin = 3;
const int gzwxCoin = 50;
const int firsetShareCoin = 20;
const int delErrCoin = -30;
const int getOneAnswerCoin = -90;
const int passAdCoin = 50;
const int promptCoin = -1;

const int PayNum = 6;
const int PayCoinMap[PayNum] = {90, 280,750,1600,3800,8000};
const int PayRmbMap[PayNum] = {2, 6 ,15,30,68,128};
#endif