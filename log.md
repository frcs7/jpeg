确定做jpeg
不知道怎么读取图片，网上查到可以用opencv读取，但用homebrew下的时候总是有问题（因为一开始想用python写，但后面还是想用c++写，中间好像装的版本有些问题，再加上出现越来越多不知道的名词，比如pip，cmake这些，所以慢慢放弃opencv），然后在网上看到可以用fp指针来读取二进制图片，于是用在csdn找了一段读取图片的代码（https://blog.csdn.net/weixin_44949552/article/details/104900498?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-4-104900498-blog-6524993.t5_download_0_7w&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-4-104900498-blog-6524993.t5_download_0_7w&utm_relevant_index=7），然后成功读取了demo.bin并读取了rgb数据并储存成了数组。数组的概念也是在这段代码里知道的，不过好在很直观，
把rgb数据转成YCrCb数据
想写一个DCT函数，最开始我想用for循环做八次，每一次把含有八个Y数据的数组做变换，然后知道原来有二维数组这个东西[ac01]
感觉这个实现并没有很复杂，就在网上找了一下DCT函数复制过来(https://blog.csdn.net/lwfcgz/article/details/8040550/)，之后发现整个程序太长了，看着很不舒服，本来我是不知道头文件这个东西的，但这几天一遍遍地报错让我大概了解了头文件是什么，就想自己做一个头文件。具体过程还是比较简单的，把函数整个复制过去就可以了，但和网上搜的做头文件的教程差了几行代码，于是群里问了一下，之后加上了。
现在要把之前转换完的YCrCb数组分成若干个8*8矩阵，但我之前用的是一维数组，所以我遇到的问题就是不知道读取的数据和二维坐标的对应关系
在qq群里问到了，我读出来的rgbrgbrgbrgb....这样一共256*256*3个数据，前256*3个属于第一行，以此类推
刚刚把Y数据用二维数组写成了一个256*256的矩阵matrixY，现在想把它分割成8*8的小矩阵。
最开始我想把这些数组一个一个存成二维数组，毕竟我之前写的DCT函数就是针对二维数组的，但发现数据量太大了。然后我参照二维数组写了一个三维数组dmatrixY，其中第一个参数表示它是第几个小块，把matrixY写了进去。
现在的问题是改写之前的DCT函数，要把它改成适用于三维数组。
9.26
函数改完了，量化的程序也写完了，但现在一直不知道写的是对的还是错的，打算写个式子先输出一部分的矩阵测试一下
我用for语句输出了一个dmatrixY矩阵，没有反应，显示zsh：abort，不知道什么原因
打算用分段注释找一下哪里有问题
找到问题了，一直到调用DCT函数之前都没有问题[ac01]，要改函数力
出现问题，我的头文件里函数定义为DCT (double data[][][]),但是我在主函数里调用，好像只能写成DCT (matrix)，不能写成DCT (matrix[][][])（查了一下好像是，一个类型是double，一个是double*，数组值和数组名的区别），但我的数组的第一个值是顺序，所以有些问题。
放弃了，找不到什么好方法对n个三维数组[n][][]依次进行调用函数，打算先把三维数组赋值给一个用于中转的二维数组，进行变换后再赋值回来
ok了，但不知道数据是对是错，也没法检验
完成了，之前写的量化的程序也可以使用，开始做差分
居然还有一个Zigzag转换，一开始想观察一下这样的下标有什么规律，下标之和依次增大，每个下标之和的个数也依次增加，但这个规律按我的代码水平不太能写出来。之后想到可以直接按图例上的路径走，其中矩阵的第一个数字和边上的数字做一点特殊处理。现在完成了，开始做差分。
程序还是很简单的，但算出来是错的[ac01]，找不到原因，我的代码是   
for (short k = 1; k <= 32*32-1; k++)

   {

      ZY[k][1] -=ZY[k-1][1];//对一维矩阵ZY的直流系数做差分

   }
找到原因了，我应该从后往前做差分的[ac01]
用git提交了，打算就用这个展示思考过程，不过git是真的难装（配置）。
no！从后往前做还是有问题！
离谱，原来原因是我以为数组的第一个元素是1了......
9.28
那个游程编码好像不能用数组储存，因为个数是未知的，查了一下打算用向量
不知道向量该怎么定义，我怎么表示次序
可以用二维向量，RLE向量里的元素是向量，但这样的话只能表示一个8*8矩阵的游程编码，我想弄出RLE1、RLE、····这样的
先做下去，可以对一个8*8矩阵一直做到huffman结束，然后再整个套一个for语句
可以用二维向量，然后再用一个数组把向量存起来，不知道向量能不能存到数组里，那这样我就不打算一次性做出所有游程编码了，直接用长编码表做完处理再存进数组里
查到可以用数组，pair，map和vector，看起来好像map更好理解一点，用这个吧
变长编码的储存既要存数字又要存字符串，不知道怎么弄，打算写个转换的函数，然后编码就继续按游程编码储存，要用的时候再调出数字套函数，最后再编起来。
打算把几个重复的部分模块化一下写成函数
想要函数的返回值是个二维数组，但好像不行，查了一下只能返回指针
tomatrix写完了，输入一维数组，可以输出一个指向二维数组的指针
测试了一下好像类似  double matrixY[256][256];

                 matrixY = tomatrix(Y);这样的代码是不可以的，我还以为指针可以整体赋值给数组
弄完了，但行不通，应该是对指针还是不熟悉，commit一次然后恢复吧
想用memcpy封装函数（用来“返回”一个数组），但失败了。想要“面向对象编程”但无奈现在代码能力还不太行。
听群里说返回值可以是vector，但我改起来时间好像不太够了，这样做起来应该会更好。
我后面用了个map来给游程编码做index，但现在想想好像vector应该可以更好地做下标，当时没想到，但ddl在即。。
我感觉把变长的游程编码写出来意义不大，因为后面处理的时候照样是需要提取出来的，所以我把游程编码用vector储存了，变长方式封装成两个函数，我觉得这样会更方便。
现在还差huffman，我的思路是用四个map去储存四个表格，然后依照map去把游程编码进行转换，但我觉得时间不太够了。



