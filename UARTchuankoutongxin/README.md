# 串口通讯
-简单介绍一下UART：这里我就直接给出我搜索的能容感觉这样更清晰<br>
UART（通用异步收发器）是单片机最常用的串口通信方式，仅需 TX（发送）、RX（接收）两根线，通过约定波特率（如 9600）、数据位等参数，实现单片机与电脑 / 外设的双向数据传输<br>
<img width="822" height="964" alt="屏幕截图 2026-02-26 103438" src="https://github.com/user-attachments/assets/20e7a5d2-0fad-407a-be76-373f69f1910d" />
<img width="730" height="184" alt="屏幕截图 2026-02-26 103526" src="https://github.com/user-attachments/assets/9a9408ff-74e4-49eb-9164-e088b0d7228f" />

- UART相应的数据格式
## 同步和异步
<img width="891" height="591" alt="屏幕截图 2026-02-26 103135" src="https://github.com/user-attachments/assets/80c0add8-c9f8-4671-8896-d36b71d3fbb6" />

## 串口通讯制式
<img width="957" height="255" alt="屏幕截图 2026-02-26 102605" src="https://github.com/user-attachments/assets/a5748def-27c8-4b79-8fb3-4dcf58bd42a4" /><br>
单工： 指数据传输仅能沿一个方向，不能实现反向传输<br>
半双工：数据传输可以沿两个方向，但需要分时<br>
全双工：数据可以同时进行双向传输<br>

## 89C52中相应通讯模块

<img width="1716" height="857" alt="屏幕截图 2026-02-26 112037" src="https://github.com/user-attachments/assets/09b11cd0-1b5b-4ec4-8cd8-c3ecc8d65fc1" /><br>

- 前3个寄存器位我们主要会使用到的寄存器下面简单介绍一下他们的功能，具体寄存器位对应的模式设置可参考相应芯片手册<br>
SCON：串口的 “控制 + 状态面板”管串口工作模式、开 / 关接收，还能看「收完数据 (RI)」「发完数据 (TI)」的状态<br>
SBUF：串口的 “数据中转站”写它 = 发数据，读它 = 收数据，一次只存 1 个字节<br>
PCON:串口的 “波特率调速键”只用管一个位（SMOD），置 1 就能让波特率翻倍
- 好现在我们看一下他的代码，初始化过程：
```c
void UART_Init()
{
	SCON=0x50;
	PCON |=0x80;
	TMOD &= 0x0F;	
	TMOD |= 0x20;		
	TL1 = 0xF3;		
	TH1 = 0xF3;		
	ET1 = 0;		
	TR1 = 1;		
	EA=1;
	ES=1;
}
```
SCON=0x50 ： 串口核心配置：
1. 工作在「模式 1」（8 位 UART，最常用）；
2. 开启接收功能（REN=1），能收串口数据	
PCON |=0x80 ： SMOD置1 波特率翻倍<br>
然后就是对定时器的设置让其提供对应的频率<br>
知道最后两行则是开启对应的中断ES=1开启串口中断
<img width="861" height="328" alt="屏幕截图 2026-02-26 114151" src="https://github.com/user-attachments/assets/026c55d6-0172-47f4-91b2-592ee44d76c4" /><br>

- 根据此图，我们可以清晰地知道其实收发数据就是对SBUF寄存器的读写
```c
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
```
- 这里放置一个循环等待发送完成，及发送完成后硬件会将TI位置1
- <img width="831" height="279" alt="屏幕截图 2026-02-26 115221" src="https://github.com/user-attachments/assets/8f3d9c8f-bf87-416f-a659-6b0ec7dc084e" />
- 很显然这里是手动设置

## 主函数
- 主函数在初始化之后就进入了死循环
- 我们看4号中断函数：
```c
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		P2=~SBUF;
		UART_SendByte(SBUF);
		RI=0;
	}
}
```
- 这里P2端口用LED来做效果验证
- 这里个中断时个时间触发型中断，TI和RI位置1会触发分被表示发送和接受，这里先判断是否为接收在做出对应的处理然后再手动将RI置0：大体效果就是电脑端向单片机发送数据然后单片机再发送相同数据给电脑
- 我们看最终效果
- <img width="799" height="570" alt="屏幕截图 2026-02-26 121224" src="https://github.com/user-attachments/assets/23976dd4-8c02-42e3-8946-68d3f6865f6b" />
- <img width="791" height="568" alt="屏幕截图 2026-02-26 121417" src="https://github.com/user-attachments/assets/096bcea8-69c2-4260-8aa9-7f3ead108c0b" />
- ![微信图片_20260226121716_64_2](https://github.com/user-attachments/assets/3c80aac2-dd1e-4932-81df-f4ea90214f66)

