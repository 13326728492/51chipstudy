# 计时器
-本次定时器包含了时钟中断模块，AT24C02芯片的存储和输出，数码管显示模块以及I2C总线模块

## 时钟中断模块
- 这简单介绍一下晶振：
它为单片机提供精准、稳定的时钟频率，决定了单片机执行指令的速度、定时器 / 串口等外设的工作时序，是整个单片机系统能有序运行的核心
- 单片机的定时中断器：
- <img width="561" height="814" alt="屏幕截图 2026-02-25 110415" src="https://github.com/user-attachments/assets/686d21d2-62c2-4a55-8408-7001e4b48163" />

- 定时器的实质是加一计数器（16位），由高八位和第八位两个寄存器组成。TMOD是定时器工作方式寄存器，确定定时器的工作方式和功能，TCON是控制寄存器，控制T0，T1的启动和停止及设置溢出标志

![微信图片_20260225111151_59_2](https://github.com/user-attachments/assets/e79ac69a-7adf-4d74-b298-1aca970badf1)
- 该计数器的计数脉冲来源一是系统时钟震荡经12分频后送来的，二是由T0，T1引脚输入的外部脉冲，这里使用的是外部脉冲
- 下面来简单讲一下这里的计数原理
![微信图片_20260225112339_60_2](https://github.com/user-attachments/assets/0e380122-d569-4937-a5a3-c1c6452bfab8)<br>
当GATE=0，TR0=1时TL0就在机器周期的作用下开始计数，当TL0计满后向TH0进位直到TH0也计满后将TF0置1，然后向CPU申请中断，定时器需要设置一个初值（也就是设置TH0和TL0的值）每次计数就在这个初值基础上进行+1操作

- 接下来介绍一下这一部分的代码
```c
void Timer0_Init(void)
{
	TMOD &= 0xF0;		
	TMOD |= 0x01;		
	TL0 = 0x18;		
	TH0 = 0xFC;		
	TF0 = 0;		
	TR0 = 1;		
	ET0=1;
	EA=1;
	PT0=0;
}
```
<br>
第一步利用与运算将第四位清0，这样做是清空就配置避免对后续产生影响<br>
第二布则是设置定时器工作模式，具体可查阅对应手册，内有寄存器每个位对应的功能说明<br>
第三则是设置定时器的初值，这里使用的是11.0592MHz的晶振，这里对应的时间为1ms<br>
第四清楚溢出位TF0 将其清0<br>
最后启动定时器并开启相应的全局中断和外设中断即分别使TR0 ET0 EA置1<br>
最后最后设置该中断的优先级PT0=0 默认低优先级

```c
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count1,T0Count2,T0Count3;
	TL0 = 0x18;
	TH0 = 0xFC;
	T0Count1++;
	if(T0Count1>=20)
	{
		T0Count1=0;
		Key_Loop();
	}
	T0Count2++;
	if(T0Count2>=2)
	{
		T0Count2=0;
		Nixie_Loop();
	}
	T0Count3++;
	if(T0Count3>=10)
	{
		T0Count3=0;
		Sec_Loop();
	}
}
```
<br>
我们在一号中断处理程序中又设置了一遍时钟的初值


## I2C总线通信模块
- 这里先简单介绍一下I2C通信协议
  它是一种两线 同步 半双工的串行通信机制
- <img width="994" height="426" alt="image" src="https://github.com/user-attachments/assets/c46eb61a-540d-43e4-9d52-ac467cd44fc9" />
- 我们现在再来看看其通信时序并展示代码
- <img width="772" height="248" alt="image" src="https://github.com/user-attachments/assets/684fcf7a-0666-40c3-8e9b-4dad57e4843e" />
```c
void I2C_Start(void)
{
	I2C_SDA=1;
	I2C_SCL=1;
	I2C_SDA=0;
	I2C_SCL=0;
}

void I2C_Stop(void)
{
	I2C_SDA=0;
	I2C_SCL=1;
	I2C_SDA=1;
}
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA=Byte&(0x80>>i);
		I2C_SCL=1;
		I2C_SCL=0;
	}
}

unsigned char I2C_ReceiveByte(void)
{
	unsigned char i,Byte=0x00;
	I2C_SDA=1;
	for(i=0;i<8;i++)
	{
		I2C_SCL=1;
		if(I2C_SDA){Byte|=(0x80>>i);}
		I2C_SCL=0;
	}
	return Byte;
}
```
- 可以很清晰的看出起始信号于终止信号的电平反转变化，根据图封装以上代码

- 应答信号
<img width="773" height="259" alt="image" src="https://github.com/user-attachments/assets/1455df5d-b960-4096-bf98-9374eb536580" /><br>
```c
void I2C_SendAck(unsigned char AckBit)
{
	I2C_SDA=AckBit;
	I2C_SCL=1;
	I2C_SCL=0;
}
```
- 再看看写时序
- <img width="783" height="561" alt="image" src="https://github.com/user-attachments/assets/81c399ba-6d5e-4120-b16e-3e516ce1c8ea" />
- 读时序
-<img width="790" height="702" alt="image" src="https://github.com/user-attachments/assets/4c7f5ea2-8624-4968-bb70-e202b580055c" />

## AT24C02模块
- 这里的代码就是以上I2C读写时序的封装软件模拟
```c
void AT24C02_WriteByte(unsigned char WordAddress,Data)
{
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS|0x01);
	I2C_ReceiveAck();
	Data=I2C_ReceiveByte();
	I2C_SendAck(1);
	I2C_Stop();
	return Data;
}
```

- 好这两个模块就暂且说到这里

## 数码管模块
- 看原理图可以清晰的知道数码管本质时一些LED显示不同的数字是不同LED亮起的组合
- 于是我们在代码中首先对不同的组合先进行封装，以便后续使用
```c
unsigned char Nixie_Buf[9]={0,10,10,10,10,10,10,10,10};  //这里设置缓冲区，让其能够一次性显示

unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};
```
- 这里要提一嘴，由于数码管一次只能显示一位，所以要一下子显示多位则要利用人的视觉暂留效应，通过循环使其不断切换从而让我们觉得又一次显示多位的错觉，这有点像CPU的多任务切换，所以这就是设置缓冲区的目的，方便后续显示使用
- 再看后续逻辑设置缓冲区就不提了非常简单，我们看看数码管扫描部分
```c
void Nixie_Scan(unsigned char Location,Number)
{
	P0=0x00;				
	switch(Location)		
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;         //位选 共有8位所以这里三位能表示8中组合
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
	}
	P0=NixieTable[Number];	
}
```
先进行清0操作，然后就是位选然后设置相应位的值
- 接着就是我们这里的循环逻辑了 由于是提供给后续逻辑使用 将循环变量i设置位静态变量从而使其后续不再一直被初始化为1，这里后续会在讲到

## 按键模块
-我们来看一下Key.c中的第一个函数这又是干什么的呢？
```c
unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}
```
这是在将键盘值清0，并返回清0之前的状态
- 而下一个函数则是获取键盘按下的状态
- 我们再看你一下最后一个函数 这里是为后续提供的循环逻辑 通过状态对比实现按键消抖和检测按键松开的动作
```c
void Key_Loop(void)
{
	static unsigned char NowState,LastState;
	LastState=NowState;				// 保存上一次的按键状态
	NowState=Key_GetState();		// 获取当前的按键状态
	
	// 检测“按键从按下状态变为松开状态”（即按键释放动作）
	if(LastState==1 && NowState==0)
	{
		Key_KeyNumber=1;
	}
	if(LastState==2 && NowState==0)
	{
		Key_KeyNumber=2;
	}
	if(LastState==3 && NowState==0)
	{
		Key_KeyNumber=3;
	}
	if(LastState==4 && NowState==0)
	{
		Key_KeyNumber=4;
	}
}
```
<br>
所以这里逻辑很简单无非就是设置了两个变量，一个用来保存上一次的按键状态，一个又来保存当前状态，然后对值进行对比然后设置Key_KeyNumber的值

## 主循环
- 现在我们来到主循环这里，这里先对时钟进行了初始化，然后就是数据存储逻辑和显示逻辑和开始计时逻辑
- 我们先看时间计时的模块Sec_Loop这里逻辑就是走玩1000ms进位走完60秒进位的意思，这里就不多做解释
- 中断1
```c
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count1,T0Count2,T0Count3;
	TL0 = 0x18;
	TH0 = 0xFC;
	T0Count1++;
	if(T0Count1>=20)
	{
		T0Count1=0;
		Key_Loop();
	}
	T0Count2++;
	if(T0Count2>=2)
	{
		T0Count2=0;
		Nixie_Loop();
	}
	T0Count3++;
	if(T0Count3>=10)
	{
		T0Count3=0;
		Sec_Loop();
	}
}
```
这样设计的原因是实现但定时器实现多任务精准调度通过不同计数器的分频满足按键秒表数码管三种不同的需求
- 这里利用视觉暂留效应这里让数码管2ms触发一次
- 根据计时规则这里计时循环10ms触发一次
- 按键20ms触发一次解决按键消抖问题，同是平衡反应速度

  







