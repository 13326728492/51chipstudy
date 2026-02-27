# 蜂鸣器播放音乐
## 蜂鸣器发生原理
- 通过交变电流让内部的振动片（或压电片）快速振动，带动空气产生声波，就是我们听到的声音
<img width="653" height="485" alt="屏幕截图 2026-02-27 130044" src="https://github.com/user-attachments/assets/9c715ba2-274a-4dad-a8ef-63e7fc1c048b" />

## PWM
- 什么是PWM脉冲宽度调制：
简单说：用一串快速开关的方波，模拟出 “不同电压 / 不同功率” 的效果。

- 好现在边看代码边做解释
```c
unsigned int FreqTable[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};
```
- 前面是一组宏定义用来定义这个数组的索引，这里封装的时一组频率，是跟据音阶设置分别是do，re，mi...
- 下面则是音乐部分通过不同音符组合从而达到播放音乐的效果，这里包含了一点乐理部分，我不多做解释

- 好我们继续讲讲播放器部分，他是如何利用PWM这个原理进行播放的呢
```c
unsigned char FreqSelect,MusicSelect;

void main()
{
	Timer0Init();
	LCD_ShowString(1,1,"HAPPY NEW YEAR");
	while(1)
	{
		if(Music[MusicSelect]!=0xFF)	//?????????
		{
			FreqSelect=Music[MusicSelect];	//?????????
			MusicSelect++;
			Delay(SPEED/4*Music[MusicSelect]);	//?????????
			MusicSelect++;
			TR0=0;
			Delay(5);	//???????
			TR0=1;
		}
		else	//????????
		{
			TR0=0;
			while(1);
		}
	}
		LCD_ShowString(1,1,"HAPPY NEW YEAR");
}

void Timer0_Routine() interrupt 1
{
	if(FreqTable[FreqSelect])	//???????
	{
		/*???????????????*/
		TL0 = FreqTable[FreqSelect]%256;		//??????
		TH0 = FreqTable[FreqSelect]/256;		//??????
		Buzzer=!Buzzer;	//?????IO?
	}
}

```
这里设置了FreqSelect变量用来对应选择推进相应的音符

- 中断一中通使用定时器来提供对应的频率，应为是震动发生要通过翻转电平来产生震动，看这个蜂鸣器的结构可知，这里就通过这个时钟中断来不断的反转电平从而产生这个震动频率从而播放出对应的音符
- 好我们看播放部分 0xff是结束符的意思写在歌曲的结束
```c
FreqSelect=Music[MusicSelect];	//?????????
			MusicSelect++;
			Delay(SPEED/4*Music[MusicSelect]);	//?????????
			MusicSelect++;
			TR0=0;
			Delay(5);	//???????
			TR0=1;
```
- 这里的逻辑也比较简单 就是个一段时间切换一下一个频率也就是音符，值得注意的是
```c
TR0=0;
Delay(5);	
TR0=1;
```
这一部分短暂关闭定时器就是让音符之间分开不要粘在一起

- 好了现在介绍完毕，烧录在单片机上就可以使用了
 
