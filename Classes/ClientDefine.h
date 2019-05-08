#ifndef _CLIENTDEFINE_H_ 
#define _CLIENTDEFINE_H_
#include <algorithm>
#include <string>
#include <map>

//namespace std;


/* 密码秘钥 */
const std::string SERVER_KEY = "l$`h.V,&";


/*LAYER tag 值*/
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
const int CODE_REG_RE = 1001; //注册
const int CODE_REG_RS = 1002; //注册返回

const int CODE_LOGIN_RE = 1003; //登录
const int CODE_LOGIN_RS = 1004; //登录返回
const int CODE_LOGIN_FROM_CENTER_RE = 1005; //远程登录

const int CODE_GAME_GET_COIN_RE = 1110;//请求金币
const int CODE_GAME_GET_COIN_RS = 1111;//请求金币返回
const int MSG_BET_SAMPLE_REQ = 1116;//请求下注请求
const int MSG_BET_SAMPLE_RSP = 1117;//请求下注请求返回
const int MSG_ODDS_SAMPLE_REQ = 1118;//普通版请求跑马倍率
const int MSG_ODDS_SAMPLE_RSP = 1119;//普通版请求跑马倍率返回
const int MSG_FRUIYS_DA_XIA_REQ = 1120;//水果机赌大小
const int MSG_FRUIYS_DA_XIA_RSP = 1121;//水果机赌大小返回






//music
const std::string BG_MUSIC = "mp3/bgm.mp3";
const std::string BUTTON_EFFECT = "mp3/button.mp3";
const std::string ANSWER_EFFECT = "mp3/click.mp3";
const std::string WIN_EFFECT = "mp3/pass.mp3";
const std::string FAILURE_EFFECT = "mp3/error.mp3";

//每日奖励
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