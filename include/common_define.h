#ifndef __COMMON_DEFINE_H_
#define __COMMON_DEFINE_H_

/*****alarm level define******/
#define LEVEL_1 SMDController::LEVEL1
#define LEVEL_2 SMDController::LEVEL2
#define LEVEL_3 SMDController::LEVEL3
#define LEVEL_4 SMDController::LEVEL4

/****************** diesel defines ******************/
#define SIGNAME_05001   "超速停机告警"
#define SIGDES_05001    "超速告警"
#define SIGID_05001     "05001"

#define SIGNAME_05002   "丢速停机告警"
#define SIGDES_05002    "油机转速过低导致保护停机"
#define SIGID_05002     "05002"

#define SIGNAME_05003   "输出低电压停机告警"
#define SIGDES_05003    "发电机组输出电压过低导致保护停机"
#define SIGID_05003     "05003"

#define SIGNAME_05004   "高水温停机告警"
#define SIGDES_05004    "油机冷却液温度过高导致保护停机"
#define SIGID_05004     "05004"

#define SIGNAME_05005   "过载停机告警"
#define SIGDES_05005    "发电机组输出过载导致保护停机"
#define SIGID_05005     "05005"

#define SIGNAME_05006   "紧急停机告警"
/***revise according to zhangli new confirm 2018 3 20***/
#define SIGDES_05006    "紧急停机告警"
#define SIGID_05006     "05006"

#define SIGNAME_05007   "冷却剂泄漏停机告警"
#define SIGDES_05007    "油机冷却液位过低导致保护停机"
#define SIGID_05007     "05007"

#define SIGNAME_05008   "低油温告警"
#define SIGDES_05008    "油机润滑油温度过低告警"
#define SIGID_05008     "05008"

#define SIGNAME_05009   "油机启动"
/***revise according to zhangli new confirm 2018 3 20***/
#define SIGDES_05009    "油机启动"
#define SIGID_05009     "05009"

#define SIGNAME_05010   "低油压告警"
#define SIGDES_05010    "低油压告警"
#define SIGID_05010     "05010"

#define SIGNAME_05011   "高油温告警"
#define SIGDES_05011    "油机润滑油温度过高告警"
#define SIGID_05011     "05011"

#define SIGNAME_05012   "发电机总告警"
#define SIGDES_05012    "发电机总告警"
#define SIGID_05012     "05012"

#define SIGNAME_05013   "高温告警"
/***revise according to zhangli new confirm 2018 3 20***/
#define SIGDES_05013    "高温告警"
#define SIGID_05013     "05013"

#define SIGNAME_05014   "水温低告警"
#define SIGDES_05014    "水温低告警"
#define SIGID_05014     "05014"

#define SIGNAME_05015   "低水位告警"
#define SIGDES_05015    "低水位告警"
#define SIGID_05015     "05015"

#define SIGNAME_05016   "启动失败故障告警"
#define SIGDES_05016    "启动失败故障告警"
#define SIGID_05016     "05016"

#define SIGNAME_05017   "控制系统告警"
#define SIGDES_05017    "发电机组控制系统故障告警"
#define SIGID_05017     "05017"

#define SIGNAME_05018   "进排风风门告警"
#define SIGDES_05018    "风门未关，设备故障等"
#define SIGID_05018     "05018"

#define SIGNAME_05019   "启动电池电压低告警"
#define SIGDES_05019    "启动电池电压低"
#define SIGID_05019     "05019"

#define SIGNAME_05020   "启动电池电压高告警"
#define SIGDES_05020    "启动电池电压高"
#define SIGID_05020     "05020"

#define SIGNAME_05021   "充电器故障告警"
#define SIGDES_05021    "充电器故障告警"
#define SIGID_05021     "05021"

#define SIGNAME_05022   "燃油油位低告警"
#define SIGDES_05022    "燃油液位低"
#define SIGID_05022     "05022"

#define SIGNAME_05023   "手动控制状态"
#define SIGDES_05023    "手动控制状态"
#define SIGID_05023     "05023"

#define SIGNAME_05024   "其他告警"
#define SIGDES_05024    "辅助告警"
#define SIGID_05024     "05024"

#define SIGNAME_05025   "输出电压过高告警"
#define SIGDES_05025    "输出电压高"
#define SIGID_05025     "05025"

#define SIGNAME_05026   "输出电压过低告警"
#define SIGDES_05026    "输出电压低"
#define SIGID_05026     "05026"

#define SIGNAME_05027   "输出过载告警"
#define SIGDES_05027    "输出过载告警"
#define SIGID_05027     "05027"

#define SIGNAME_05028   "输出频率高告警"
#define SIGDES_05028    "输出频率高"
#define SIGID_05028     "05028"

#define SIGNAME_05029   "输出频率低告警"
#define SIGDES_05029    "输出频率低"
#define SIGID_05029     "05029"

#define SIGNAME_05030   "低油压停机告警"
#define SIGDES_05030    "低油压停机告警"
#define SIGID_05030     "05030"

#define SIGNAME_05031   "市电故障告警状态"
#define SIGDES_05031    "市电供电有故障"
#define SIGID_05031     "05031"

#define SIGNAME_05032   "市电电压过高告警"
#define SIGDES_05032    "市电电压高"
#define SIGID_05032     "05032"

#define SIGNAME_05033   "市电电压过低告警"
#define SIGDES_05033    "市电电压低"
#define SIGID_05033     "05033"

#define SIGNAME_05034   "市电频率高告警"
#define SIGDES_05034    "市电频率高"
#define SIGID_05034     "05034"

#define SIGNAME_05035   "市电频率低告警"
#define SIGDES_05035    "市电频率低"
#define SIGID_05035     "05035"

#define SIGNAME_05036   "油机供电告警状态"
#define SIGDES_05036    "油机对负载供电"
#define SIGID_05036     "05036"

/********************开关电源 ***********************************/
#define SIGNAME_06001   "电池熔丝故障告警"
#define SIGDES_06001    "电池熔丝故障告警"
#define SIGID_06001     "06001"
#define SIGLEVEL_06001  1   //一级告警

#define SIGNAME_06002   "电池充电过流告警"
#define SIGDES_06002    "电池充电过流告警状态"
#define SIGID_06002     "06002"
#define SIGLEVEL_06002  3   //三级告警

#define SIGNAME_06003   "电池温度过高告警"
#define SIGDES_06003    "电池组温度高于设定告警阈值"
#define SIGID_06003     "06003"
#define SIGLEVEL_06003  3   //三级告警

#define SIGNAME_06004   "电池放电不平衡告警"
#define SIGDES_06004    "电池放电不平衡(1组<2组/(1组>2组)"
#define SIGID_06004     "06004"
#define SIGLEVEL_06004  4   //四级告警

#define SIGNAME_06005   "电池供电告警"
#define SIGDES_06005    "开关电源处于电池供电中"
#define SIGID_06005     "06005"
#define SIGLEVEL_06005  2   //二级告警

#define SIGNAME_06006   "负载电流过高告警"
#define SIGDES_06006    "遥测值,若负载总电流高于设定告警阈值"
#define SIGID_06006     "06006"
#define SIGLEVEL_06006  4   //四级告警

#define SIGNAME_06007   "负载熔丝故障告警"
#define SIGDES_06007    "负载熔丝或空开故障,引起负载供电中断"
#define SIGID_06007     "06007"
#define SIGLEVEL_06007  1   //一级告警

#define SIGNAME_06008   "输出电压过低告警"
#define SIGDES_06008    "输出电压过低告警"
#define SIGID_06008     "06008"
#define SIGLEVEL_06008  3   //三级告警

#define SIGNAME_06009   "输出电压过高告警"
#define SIGDES_06009    "输出电压过高告警"
#define SIGID_06009     "06009"
#define SIGLEVEL_06009  3   //"三级告警"

#define SIGNAME_06010   "输出中断告警"
#define SIGDES_06010    "输出中断"
#define SIGID_06010     "06010"
#define SIGLEVEL_06010  1   //一级告警

#define SIGNAME_06011   "直流屏通讯中断告警"
#define SIGDES_06011    "直流屏通讯中断告警状态"
#define SIGID_06011     "06011"
#define SIGLEVEL_06011  4   //四级告警

#define SIGNAME_06012   "监控模块故障告警"
#define SIGDES_06012    "监控模块故障告警"
#define SIGID_06012     "06012"
#define SIGLEVEL_06012  2   //二级告警

#define SIGNAME_06013   "交流输入空开跳告警"
#define SIGDES_06013    "交流输出分路断开"
#define SIGID_06013     "06013"
#define SIGLEVEL_06013  1   //一级告警

#define SIGNAME_06014   "交流输入电压过高告警"
#define SIGDES_06014    "交流输入电压超过阈值"
#define SIGID_06014     "06014"
#define SIGLEVEL_06014  4   //四级告警

#define SIGNAME_06015   "交流输入电压过低告警"
#define SIGDES_06015    "交流输入电压超过阈值"
#define SIGID_06015     "06015"
#define SIGLEVEL_06015  4   //四级告警

#define SIGNAME_06016   "交流输入停电告警"
#define SIGDES_06016    "交流输入停电"
#define SIGID_06016     "06016"
#define SIGLEVEL_06016  3   //三级告警

#define SIGNAME_06017   "交流输入缺相告警"
#define SIGDES_06017    "交流输入缺相"
#define SIGID_06017     "06017"
#define SIGLEVEL_06017  3   //三级告警

#define SIGNAME_06018   "交流输入频率过高告警"
#define SIGDES_06018    "交流输入频率越限告警"
#define SIGID_06018     "06018"
#define SIGLEVEL_06018  4   //四级告警

#define SIGNAME_06019   "交流输入频率过低告警"
#define SIGDES_06019    "交流输入频率越限告警"
#define SIGID_06019     "06019"
#define SIGLEVEL_06019  4   //四级告警

#define SIGNAME_06020   "市电切换失败故障告警"
#define SIGDES_06020    "市电切换失败故障告警状态"
#define SIGID_06020     "06020"
#define SIGLEVEL_06020  4   //四级告警

#define SIGNAME_06021   "防雷器空开断开告警"
#define SIGDES_06021    "防雷器空开断开"
#define SIGID_06021     "06021"
#define SIGLEVEL_06021  2   //二级告警

#define SIGNAME_06022   "防雷器故障告警"
#define SIGDES_06022    "交流防雷器断故障告警状态"
#define SIGID_06022     "06022"
#define SIGLEVEL_06022  2   //二级告警

#define SIGNAME_06023   "交流屏通讯中断告警"
#define SIGDES_06023    "交流屏通讯中断告警"
#define SIGID_06023     "06023"
#define SIGLEVEL_06023  4   //四级告警

#define SIGNAME_06024   "整流模块故障告警"
#define SIGDES_06024    "模块故障告警状态"
#define SIGID_06024     "06024"
#define SIGLEVEL_06024  2   //二级告警

#define SIGNAME_06025   "整流模块风扇告警"
#define SIGDES_06025    "模块风扇告警"
#define SIGID_06025     "06025"
#define SIGLEVEL_06025  2   //二级告警

#define SIGNAME_06026   "整流模块过压关机告警"
#define SIGDES_06026    "整流模块过压关机告警"
#define SIGID_06026     "06026"
#define SIGLEVEL_06026  2   //二级告警

#define SIGNAME_06027   "整流模块温度过高告警"
#define SIGDES_06027    "模块过温告警状态"
#define SIGID_06027     "06027"
#define SIGLEVEL_06027  2   //二级告警

#define SIGNAME_06028   "整流模块通信状态告警"
#define SIGDES_06028    "模块通讯中断告警状态"
#define SIGID_06028     "06028"
#define SIGLEVEL_06028  2   //二级告警

#define SIGNAME_06029   "其他告警"
#define SIGDES_06029    "其他告警"
#define SIGID_06029     "06029"
#define SIGLEVEL_06029  3   //三级告警

#define SIGNAME_06079   "一级低压脱离告警"
#define SIGDES_06079    "第一级低压脱离开关动作，其后的负载被断电"
#define SIGID_06079     "06079"
#define SIGLEVEL_06079  1   //一级告警

#define SIGNAME_06080   "二级低压脱离告警"
#define SIGDES_06080    "第二级低压脱离开关动作，其后的负载被断电"
#define SIGID_06080     "06080"
#define SIGLEVEL_06080  1   //一级告警

/**defined  by tu**/
#define SIGNAME_06030    "电池温度补偿功能启用"
#define SIGDES_06030     "其他告警"
#define SIGID_06030      "06030"
#define SIGLEVEL_06030   3

#define SIGNAME_06031   "电池放电"
#define SIGDES_06031    "其他告警"
#define SIGID_06031     "06031"
#define SIGLEVEL_06031  3

#define SIGNAME_06032   "整流模块故障（整流模块关机）"
#define SIGDES_06032   "其他告警"
#define SIGID_06032     "06032"
#define SIGLEVEL_06032  3

#define SIGNAME_06033   "整流模块告警（整流模块没有关机的告警）"
#define SIGDES_06033   "其他告警"
#define SIGID_06033     "06033"
#define SIGLEVEL_06033  3

#define SIGNAME_06034   "交流输入故障"
#define SIGDES_06034   "其他告警"
#define SIGID_06034     "06034"
#define SIGLEVEL_06034  3

#define SIGNAME_06038   "电池开关断开"
#define SIGDES_06038   "其他告警"
#define SIGID_06038     "06038"
#define SIGLEVEL_06038  3

#define SIGNAME_06039   "环境温度过高"
#define SIGDES_06039   "其他告警"
#define SIGID_06039     "06039"
#define SIGLEVEL_06039  3

#define SIGNAME_06040   "低压断路开关（LVDS）闭合"
#define SIGDES_06040   "其他告警"
#define SIGID_06040     "06040"
#define SIGLEVEL_06040  3

#define SIGNAME_06041   "低压断路开关（LVDS）断开"
#define SIGDES_06041   "其他告警"
#define SIGID_06041     "06041"
#define SIGLEVEL_06041  3

#define SIGNAME_06042   "系统输出电压过高"
#define SIGDES_06042   "其他告警"
#define SIGID_06042     "06042"
#define SIGLEVEL_06042  3

#define SIGNAME_06043   "系统输出电压过低"
#define SIGDES_06043   "其他告警"
#define SIGID_06043     "06043"
#define SIGLEVEL_06043  3

#define SIGNAME_06044   "整流模块主要告警（两个或两个以上的模块关机）"
#define SIGDES_06044   "其他告警"
#define SIGID_06044     "06044"
#define SIGLEVEL_06044  3

#define SIGNAME_06045   "整流模块次要告警（一个模块关机）"
#define SIGDES_06045    "其他告警"
#define SIGID_06045     "06045"
#define SIGLEVEL_06045  3

#define SIGNAME_06046   "整流模块警告（没有导致关机的告警）"
#define SIGDES_06046    "其他告警"
#define SIGID_06046     "06046"
#define SIGLEVEL_06046  3

#define SIGNAME_06047   "霍尔xx告警"
#define SIGDES_06047    "其他告警"
#define SIGID_06047     "06047"
#define SIGLEVEL_06047  3

#define SIGNAME_06050   "内部高压直流回路故障"
#define SIGDES_06050    "其他告警"
#define SIGID_06050     "06050"
#define SIGLEVEL_06050  3

#define SIGNAME_06051   "电压回路控制放大器超出正常工作范围"
#define SIGDES_06051   "其他告警"
#define SIGID_06051     "06051"
#define SIGLEVEL_06051  3

#define SIGNAME_06052   "内部温度高"
#define SIGDES_06052   "其他告警"
#define SIGID_06052     "06052"
#define SIGLEVEL_06052  3

#define SIGNAME_06053   "内部控制回路调节电压故障"
#define SIGDES_06053   "其他告警"
#define SIGID_06053     "06053"
#define SIGLEVEL_06053  3

#define SIGNAME_06058   "参考电压超出范围"
#define SIGDES_06058   "其他告警"
#define SIGID_06058     "06058"
#define SIGLEVEL_06058  3

#define SIGNAME_06059   "交流故障"
#define SIGDES_06059   "其他告警"
#define SIGID_06059     "06059"
#define SIGLEVEL_06059  3

#define SIGNAME_06061   "均衡充电"
#define SIGDES_06061   "其他告警"
#define SIGID_06061     "06061"
#define SIGLEVEL_06061  3

#define SIGNAME_06062   "故障并关机"
#define SIGDES_06062   "其他告警"
#define SIGID_06062     "06062"
#define SIGLEVEL_06062  3

#define SIGNAME_06063   "告警未关机"
#define SIGDES_06063   "其他告警"
#define SIGID_06063     "06063"
#define SIGLEVEL_06063  3

#define SIGNAME_06065   "整流模块非紧急告警"
#define SIGDES_06065    "其他告警"
#define SIGID_06065     "06065"
#define SIGLEVEL_06065  3

#define SIGNAME_06066   "整流模块紧急告警"
#define SIGDES_06066    "其他告警"
#define SIGID_06066     "06066"
#define SIGLEVEL_06066  3

#define SIGNAME_06068   "交流输入电压告警"
#define SIGDES_06068    "其他告警"
#define SIGID_06068     "06068"
#define SIGLEVEL_06068  3

#define SIGNAME_06069   "交流输入频率告警"
#define SIGDES_06069    "其他告警"
#define SIGID_06069     "06069"
#define SIGLEVEL_06069  3

#define SIGNAME_06070   "电池温度传感器故障"
#define SIGDES_06070    "其他告警"
#define SIGID_06070     "06070"
#define SIGLEVEL_06070  3

#define SIGNAME_06071   "整流模块参数超出范围"
#define SIGDES_06071    "其他告警"
#define SIGID_06071     "06071"
#define SIGLEVEL_06071  3

#define SIGNAME_06072   "模块输出电压低"
#define SIGDES_06072    "其他告警"
#define SIGID_06072     "06072"
#define SIGLEVEL_06072  3

#define SIGNAME_06073   "通信故障"
#define SIGDES_06073    "其他告警"
#define SIGID_06073     "06073"
#define SIGLEVEL_06073  3

#define SIGNAME_06074   "交流断路器断开"
#define SIGDES_06074    "其他告警"
#define SIGID_06074     "06074"
#define SIGLEVEL_06074  3

#define SIGNAME_06075   "电池熔断器故障"
#define SIGDES_06075    "其他告警"
#define SIGID_06075     "06075"
#define SIGLEVEL_06075  3

#define SIGNAME_06076   "负载熔断器故障"
#define SIGDES_06076    "其他告警"
#define SIGID_06076     "06076"
#define SIGLEVEL_06076  3

#define SIGNAME_06077   "电池容量低"
#define SIGDES_06077   "其他告警"
#define SIGID_06077     "06077"
#define SIGLEVEL_06077  3

#define SIGNAME_06078   "电池电压低于系统电压"
#define SIGDES_06078   "其他告警"
#define SIGID_06078     "06078"
#define SIGLEVEL_06078  3

#define SIGNAME_06081   "蓄电池故障"
#define SIGDES_06081   "其他告警"
#define SIGID_06081     "06081"
#define SIGLEVEL_06081  3

#define SIGNAME_06082   "电池回路断"
#define SIGDES_06082   "其他告警"
#define SIGID_06082     "06082"
#define SIGLEVEL_06082  3

/******************** ups *****************************/
#define SIGNAME_08001   "交流输入中断告警"
#define SIGDES_08001    "交流输入中断"
#define SIGID_08001     "08001"

#define SIGNAME_08002   "交流输入电压过高告警"
#define SIGDES_08002    "交流输入电压超过阈值"
#define SIGID_08002     "08002"

#define SIGNAME_08003   "交流输入电压过低告警"
#define SIGDES_08003    "交流输入电压超过阈值"
#define SIGID_08003     "08003"

#define SIGNAME_08004   "交流输入缺相告警"
#define SIGDES_08004    "交流输入缺相"
#define SIGID_08004     "08004"

#define SIGNAME_08005   "交流输入频率过高告警"
#define SIGDES_08005    "交流交流输入频率超过设定阈值范围"
#define SIGID_08005     "08005"

#define SIGNAME_08006   "交流输入频率过低告警"
#define SIGDES_08006    "交流交流输入频率超过设定阈值范围"
#define SIGID_08006     "08006"

#define SIGNAME_08007   "旁路输入中断告警"
#define SIGDES_08007    "旁路输入中断"
#define SIGID_08007     "08007"

#define SIGNAME_08008   "旁路输入电压过高告警"
#define SIGDES_08008    "旁路输入电压超过阈值"
#define SIGID_08008     "08008"

#define SIGNAME_08009   "旁路输入电压过低告警"
#define SIGDES_08009    "旁路输入电压超过阈值"
#define SIGID_08009     "08009"

#define SIGNAME_08010   "旁路输入缺相告警"
#define SIGDES_08010    "旁路输入缺相"
#define SIGID_08010     "08010"

#define SIGNAME_08011   "旁路输入频率过高告警"
#define SIGDES_08011    "旁路交流输入频率超过设定阈值范围"
#define SIGID_08011     "08011"

#define SIGNAME_08012   "旁路输入频率过低告警"
#define SIGDES_08012    "旁路交流输入频率超过设定阈值范围"
#define SIGID_08012     "08012"

#define SIGNAME_08013   "输出故障告警"
#define SIGDES_08013    "输出故障告警"
#define SIGID_08013     "08013"

#define SIGNAME_08014   "输出电压过高告警"
#define SIGDES_08014    "输出电压超过设定阈值"
#define SIGID_08014     "08014"

#define SIGNAME_08015   "输出电压过低告警"
#define SIGDES_08015    "输出电压低于设定阈值"
#define SIGID_08015     "08015"

#define SIGNAME_08016   "输出中断告警"
#define SIGDES_08016    "输出中断告警"
#define SIGID_08016     "08016"

#define SIGNAME_08017   "输出频率过高告警"
#define SIGDES_08017    "输出频率超过设定阈值范围"
#define SIGID_08017     "08017"

#define SIGNAME_08018   "输出频率过低告警"
#define SIGDES_08018    "输出频率超过设定阈值范围"
#define SIGID_08018     "08018"

#define SIGNAME_08019   "输出电流过载告警"
#define SIGDES_08019    "输出电流超过设备额定容量"
#define SIGID_08019     "08019"

#define SIGNAME_08020   "旁路运行状态告警"
#define SIGDES_08020    "UPS处于旁路运行状态"
#define SIGID_08020     "08020"

#define SIGNAME_08021   "旁路故障告警"
#define SIGDES_08021    "旁路开关发生故障跳闸"
#define SIGID_08021     "08021"

#define SIGNAME_08022   "旁路不可用告警"
#define SIGDES_08022    "旁路电源指标超过设备设定阈值范围导致旁路跳转功能被锁止"
#define SIGID_08022     "08022"

#define SIGNAME_08023   "旁路电压过高"
#define SIGDES_08023    "旁路电源指标超过设备设定阈值范围导致旁路跳转功能被锁止"
#define SIGID_08023     "08023"

#define SIGNAME_08024   "旁路电压过低"
#define SIGDES_08024    "旁路电源指标超过设备设定阈值范围导致旁路跳转功能被锁止"
#define SIGID_08024     "08024"

#define SIGNAME_08025   "旁路频率过高"
#define SIGDES_08025    "旁路频率超限告警"
#define SIGID_08025     "08025"

#define SIGNAME_08026   "旁路频率过低"
#define SIGDES_08026    "旁路频率超限告警"
#define SIGID_08026     "08026"

#define SIGNAME_08027   "手动旁路告警"
#define SIGDES_08027    "手动"
#define SIGID_08027     "08027"

#define SIGNAME_08028   "整流器故障告警"
#define SIGDES_08028    "整流器发生故障"
#define SIGID_08028     "08028"

#define SIGNAME_08029   "逆变器交流电压过高告警"
#define SIGDES_08029    "逆变器交流电压过高告警"
#define SIGID_08029     "08029"

#define SIGNAME_08030   "逆变器交流电压过低告警"
#define SIGDES_08030    "逆变器交流电压过低告警"
#define SIGID_08030     "08030"

#define SIGNAME_08031   "逆变器频率超限告警"
#define SIGDES_08031    "逆变器频率超限告警"
#define SIGID_08031     "08031"

#define SIGNAME_08032   "逆变器不同步告警"
#define SIGDES_08032    "逆变器不同步告警"
#define SIGID_08032     "08032"

#define SIGNAME_08033   "逆变器输入电压超限告警"
#define SIGDES_08033    "逆变器输入电压超限告警"
#define SIGID_08033     "08033"

#define SIGNAME_08034   "逆变器故障告警"
#define SIGDES_08034    "逆变器发生故障，关断"
#define SIGID_08034     "08034"

#define SIGNAME_08035   "风扇故障告警"
#define SIGDES_08035    "风扇故障告警"
#define SIGID_08035     "08035"

#define SIGNAME_08036   "熔丝故障告警"
#define SIGDES_08036    "熔丝故障告警"
#define SIGID_08036     "08036"

#define SIGNAME_08037   "电池供电告警"
#define SIGDES_08037    "电池供电"
#define SIGID_08037     "08037"

#define SIGNAME_08038   "直流电压低告警"
#define SIGDES_08038    "直流电压低告警"
#define SIGID_08038     "08038"

#define SIGNAME_08039   "系统异常告警"
#define SIGDES_08039    "系统异常告警"
#define SIGID_08039     "08039"

#define SIGNAME_08040   "电池电压低告警"
#define SIGDES_08040    "放电过程中后备电池组总电压低于设定阈值"
#define SIGID_08040     "08040"

#define SIGNAME_08041   "温度过高告警"
#define SIGDES_08041    "设备整流器、逆变器、变压器等功率器件运行温度超过设定阈值"
#define SIGID_08041     "08041"

#define SIGNAME_08042   "即将停机告警"
#define SIGDES_08042    "电池电压低告警"
#define SIGID_08042     "08042"

#define SIGNAME_08043   "温感器故障告警"
#define SIGDES_08043    "设备内部的某个温度传感器发生故障"
#define SIGID_08043     "08043"

#define SIGNAME_08044   "传感器故障告警"
#define SIGDES_08044    "设备内部的非温度传感器发生故障"
#define SIGID_08044     "08044"

#define SIGNAME_08045   "并机系统故障告警"
#define SIGDES_08045    "并机系统故障"
#define SIGID_08045     "08045"

#define SIGNAME_08046   "电池接触器故障告警"
#define SIGDES_08046    "后备电池组接入的接触器发生故障"
#define SIGID_08046     "08046"

#define SIGNAME_08047   "充电器故障告警"
#define SIGDES_08047    "充电器故障告警"
#define SIGID_08047     "08047"

#define SIGNAME_08048   "开关断开/闭合告警"
#define SIGDES_08048    "开关断开/闭合告警"
#define SIGID_08048     "08048"

#define SIGNAME_08049   "其他告警"
#define SIGDES_08049    "未列入以上告警的告警信息"
#define SIGID_08049     "08049"


/******************** air condition 专用空调(风冷) *****************************/

/**********************************************************************************************
acoording to zhangli:"只要是关于湿度和过滤网的,都可以不配告警,其他的配成二级以上告警
有些空调“过滤网”和“气流丢失”是一个信号量，这种的要配成二级告警"

关于空调过滤网、加湿、低湿、低温等相关的告警可设为四级, 不用需要派单

加湿和低湿的都不用设告警
*************************************************************************************************/

#define SIGNAME_12001   "风机过载告警"
#define SIGDES_12001    "室内风机系统的电机过载"
#define SIGID_12001     "12001"

#define SIGNAME_12002   "气流丢失告警"
#define SIGDES_12002    "室内机系统的送风气流量不足（室内机门被打开或密封破坏）"
#define SIGID_12002     "12002"

#define SIGNAME_12003   "空气过滤网脏告警"
#define SIGDES_12003    "室内机系统的过滤网脏堵"
#define SIGID_12003     "12003"

#define SIGNAME_12004   "湿度过高告警"
#define SIGDES_12004    "空调机的回风湿度高于设定阈值"
#define SIGID_12004     "12004"

#define SIGNAME_12005   "湿度过低告警"
#define SIGDES_12005    "空调机的回风湿度低于设定阈值"
#define SIGID_12005     "12005"

#define SIGNAME_12006   "温度过高告警"
#define SIGDES_12006    "空调机的回风温度超过设定阈值"
#define SIGID_12006     "12006"

#define SIGNAME_12007   "温度过低告警"
#define SIGDES_12007    "空调机的回风温度低于设定阈值"
#define SIGID_12007     "12007"

#define SIGNAME_12008   "压缩机高压告警"
#define SIGDES_12008    "空调系统的压缩机高压告警"
#define SIGID_12008     "12008"

#define SIGNAME_12009   "压缩机低压告警"
#define SIGDES_12009    "空调系统的压缩机低压告警"
#define SIGID_12009     "12009"

#define SIGNAME_12010   "压缩机短振告警"
#define SIGDES_12010    "压缩机短振告警"
#define SIGID_12010     "12010"

#define SIGNAME_12011   "压缩机过热告警"
#define SIGDES_12011    "压缩机过热告警"
#define SIGID_12011     "12011"

#define SIGNAME_12012   "压缩机过载告警"
#define SIGDES_12012    "压缩机过载停机"
#define SIGID_12012     "12012"

#define SIGNAME_12013   "系统告警"
#define SIGDES_12013    "空调控制系统发生故障"
#define SIGID_12013     "12013"

#define SIGNAME_12014   "加湿器告警"
#define SIGDES_12014    "空调系统加湿罐结垢或加湿水脏告警"
#define SIGID_12014     "12014"

#define SIGNAME_12015   "溢水告警"
#define SIGDES_12015    "加湿罐漏水"
#define SIGID_12015     "12015"

#define SIGNAME_12016   "水浸告警"
#define SIGDES_12016    "加湿罐外接的进水管路漏水"
#define SIGID_12016     "12016"

#define SIGNAME_12017   "其他告警"
#define SIGDES_12017    "未列入以上告警的告警信息"
#define SIGID_12017     "12017"

#define SIGNAME_12018   "交流输入异常告警"
#define SIGDES_12018    "反相或电压下降输入电源电压不平衡告警"
#define SIGID_12018     "12018"

/******* 中央空调（水冷） ************/
#define SIGNAME_13001   "水流开关故障告警"
#define SIGDES_13001    "水流开关故障告警"
#define SIGID_13001     "13001"

#define SIGNAME_13002   "高压开关故障告警"
#define SIGDES_13002    "高压开关故障告警"
#define SIGID_13002     "13002"

#define SIGNAME_13003   "油位开关故障告警"
#define SIGDES_13003    "油位开关故障告警"
#define SIGID_13003     "13003"

#define SIGNAME_13004   "排气压力过高告警"
#define SIGDES_13004    "排气压力过高告警"
#define SIGID_13004     "13004"

#define SIGNAME_13005   "蒸发压力过低告警"
#define SIGDES_13005    "蒸发压力过低告警"
#define SIGID_13005     "13005"

#define SIGNAME_13006   "排气温度过高告警"
#define SIGDES_13006    "排气温度过高告警"
#define SIGID_13006     "13006"

#define SIGNAME_13007   "冷却水出水温度过高告警"
#define SIGDES_13007    "冷却水出水温度过高告警"
#define SIGID_13007     "13007"

#define SIGNAME_13008   "冷冻水出水温度过低告警"
#define SIGDES_13008    "冷冻水出水温度过低告警"
#define SIGID_13008     "13008"

#define SIGNAME_13009   "油压差过低告警"
#define SIGDES_13009    "油压差过低告警"
#define SIGID_13009     "13009"

#define SIGNAME_13010   "冷却塔水位过低告警"
#define SIGDES_13010    "冷却塔水位过低告警"
#define SIGID_13010     "13010"

#define SIGNAME_13011   "冷却塔水位过高告警"
#define SIGDES_13011    "冷却塔水位过高告警"
#define SIGID_13011     "13011"

#define SIGNAME_13012   "油过滤器阻塞告警"
#define SIGDES_13012    "油过滤器阻塞告警"
#define SIGID_13012     "13012"

#define SIGNAME_13013   "传感器故障告警"
#define SIGDES_13013    "传感器故障告警"
#define SIGID_13013     "13013"

#define SIGNAME_13014   "通信状态告警"
#define SIGDES_13014    "通信状态告警"
#define SIGID_13014     "13014"

#define SIGNAME_13015   "外部联锁故障告警"
#define SIGDES_13015    "外部联锁故障告警"
#define SIGID_13015     "13015"

#define SIGNAME_13016   "压缩机启动失败故障告警"
#define SIGDES_13016    "压缩机启动失败故障告警"
#define SIGID_13016     "13016"

#define SIGNAME_13017   "压缩机内置保护告警"
#define SIGDES_13017    "压缩机内置保护告警"
#define SIGID_13017     "13017"

#define SIGNAME_13018   "压缩机电流过高告警"
#define SIGDES_13018    "压缩机电流过高告警"
#define SIGID_13018     "13018"

#define SIGNAME_13019   "压缩机电流过低告警"
#define SIGDES_13019    "压缩机电流过低告警"
#define SIGID_13019     "13019"

#define SIGNAME_13020   "通信状态告警"
#define SIGDES_13020    "通信状态告警"
#define SIGID_13020     "13020"

#define SIGNAME_13021   "其他告警"
#define SIGDES_13021    "其他告警"
#define SIGID_13021     "13021"

/*************************** 240V直流电源 **************************************/
#define SIGNAME_11001   "电池熔丝故障告警"
#define SIGDES_11001    "电池熔丝故障告警"
#define SIGID_11001     "11001"

#define SIGNAME_11002   "电池充电过流告警"
#define SIGDES_11002    "电池充电过流告警"
#define SIGID_11002     "11002"

#define SIGNAME_11003   "电池温度过高告警"
#define SIGDES_11003    "电池温度过高告警"
#define SIGID_11003     "11003"

#define SIGNAME_11004   "电池放电不平衡告警"
#define SIGDES_11004    "电池放电不平衡告警"
#define SIGID_11004     "11004"

#define SIGNAME_11005   "电池供电告警"
#define SIGDES_11005    "电池供电告警"
#define SIGID_11005     "11005"

#define SIGNAME_11006   "负载电流过高告警"
#define SIGDES_11006    "负载电流过高告警"
#define SIGID_11006     "11006"

#define SIGNAME_11007   "负载熔丝故障告警"
#define SIGDES_11007    "负载熔丝故障告警"
#define SIGID_11007     "11007"

#define SIGNAME_11008   "输出电压过低告警"
#define SIGDES_11008    "输出电压过低告警"
#define SIGID_11008     "11008"

#define SIGNAME_11009   "输出电压过高告警"
#define SIGDES_11009    "输出电压过高告警"
#define SIGID_11009     "11009"

#define SIGNAME_11010   "交流输入空开跳告警"
#define SIGDES_11010    "交流输入空开跳告警"
#define SIGID_11010     "11010"

#define SIGNAME_11011   "交流输入电压过高告警"
#define SIGDES_11011    "交流输入电压过高告警"
#define SIGID_11011     "11011"

#define SIGNAME_11012   "交流输入电压过低告警"
#define SIGDES_11012    "交流输入电压过低告警"
#define SIGID_11012     "11012"

#define SIGNAME_11013   "交流输入停电告警"
#define SIGDES_11013    "交流输入停电告警"
#define SIGID_11013     "11013"

#define SIGNAME_11014   "交流输入缺相告警"
#define SIGDES_11014    "交流输入缺相告警"
#define SIGID_11014     "11014"

#define SIGNAME_11015   "交流输入频率过高告警"
#define SIGDES_11015    "交流输入频率过高告警"
#define SIGID_11015     "11015"

#define SIGNAME_11016   "交流输入频率过低告警"
#define SIGDES_11016    "交流输入频率过低告警"
#define SIGID_11016     "11016"

#define SIGNAME_11017   "防雷器空开断开告警"
#define SIGDES_11017    "防雷器空开断开告警"
#define SIGID_11017     "11017"

#define SIGNAME_11018   "防雷器故障告警"
#define SIGDES_11018    "防雷器故障告警"
#define SIGID_11018     "11018"

#define SIGNAME_11019   "整流模块故障告警"
#define SIGDES_11019    "整流模块故障告警"
#define SIGID_11019     "11019"

#define SIGNAME_11020   "整流模块风扇告警"
#define SIGDES_11020    "整流模块风扇告警"
#define SIGID_11020     "11020"

#define SIGNAME_11021   "整流模块过压关机告警"
#define SIGDES_11021    "整流模块过压关机告警"
#define SIGID_11021     "11021"

#define SIGNAME_11022   "整流模块温度过高告警"
#define SIGDES_11022    "整流模块温度过高告警"
#define SIGID_11022     "11022"

#define SIGNAME_11023   "整流模块通信状态告警"
#define SIGDES_11023    "整流模块通信状态告警"
#define SIGID_11023     "11023"

#define SIGNAME_11024   "系统绝缘告警"
#define SIGDES_11024    "系统绝缘告警"
#define SIGID_11024     "11024"

#define SIGNAME_11025   "支路绝缘告警"
#define SIGDES_11025    "支路绝缘告警"
#define SIGID_11025     "11025"

#define SIGNAME_11026   "其他告警"
#define SIGDES_11026    "其他告警"
#define SIGID_11026     "11026"

/********************* 智能电表 ********************************************/
#define SIGNAME_16001   "输入状态停电"
#define SIGDES_16001    "输入状态停电"
#define SIGID_16001     "16001"

#define SIGNAME_16002   "流输入状态缺一项"
#define SIGDES_16002    "流输入状态缺一项"
#define SIGID_16002     "16002"

#define SIGNAME_16003   "交流输入状态缺两项"
#define SIGDES_16003    "交流输入状态缺两项"
#define SIGID_16003     "16003"

#define SIGNAME_16004   "交流输入频率过高"
#define SIGDES_16004    "交流输入频率过高"
#define SIGID_16004     "16004"

#define SIGNAME_16005   "交流输入频率过低"
#define SIGDES_16005    "交流输入频率过低"
#define SIGID_16005     "16005"

#define SIGNAME_16006   "A相电压过高"
#define SIGDES_16006    "A相电压过高"
#define SIGID_16006     "16006"

#define SIGNAME_16007   "A相电压过低"
#define SIGDES_16007    "A相电压过低"
#define SIGID_16007     "16007"

#define SIGNAME_16008   "B相电压过高"
#define SIGDES_16008    "B相电压过高"
#define SIGID_16008     "16008"

#define SIGNAME_16009   "B相电压过低"
#define SIGDES_16009    "B相电压过低"
#define SIGID_16009     "16009"

#define SIGNAME_16010   "C相电压过高"
#define SIGDES_16010    "C相电压过高"
#define SIGID_16010     "16010"

#define SIGNAME_16011   "C相电压过低"
#define SIGDES_16011    "C相电压过低"
#define SIGID_16011     "16011"

#define SIGNAME_16012   "其他告警"
#define SIGDES_16012    "其他告警"
#define SIGID_16012     "16012"


enum AC_Control_Code {
    AC_START=100, AC_START_RESULT=101, AC_STOP=102, AC_STOP_RESULT=103, AC_MUTE_ALARM=104, AC_MUTE_ALARM_RESULT=105, AC_SET_TEMPERATURE=106, AC_SET_TEMPERATURE_RESULT=107,AC_SET_WIND=108,AC_SET_WIND_RESULT=109,AC_SET_HUMID=110,AC_SET_HUMID_RESULT=111
};

#endif
