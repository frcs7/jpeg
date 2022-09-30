其实有一个log文档让我可以很轻松地总结我的思考过程。
24号刚看到任务的时候，我还是一个什么都不懂的纯萌新，对编程最多的了解就是翁凯的慕课，我看到了switch case语句（笑），所以最开始我对五个题目是完完全全懵的，因为每一个我都不知道从哪里下手。
既然没有思路，我就挑了一个看起来最有趣的，选择了cyber prison，因为我对这个小游戏还是挺感兴趣的。可是当我去搜索相关资料的时候，发现相关知识和我已有的知识是完全断层的，我根本无法理解那些教学贴里的内容，于是我转向另一个项目，jpeg，原因是它看起来似乎更考察数学而非计算机知识，我觉得可能会更加容易上手相关知识。
最开始的问题，也可能是整个项目中我觉得最直接的问题，就是文档的读取，因为最开始的我什么都不懂，我不知道该怎么读取二进制文件，不过好在这是一个很容易搜到答案的问题，我在网上找到了一段统计图片文件中rgb数据的熵的代码，提取了最前面一段，将rgb数据读了出来。从这个过程里我也模糊地了解了一些概念，比如指针等。
之后的过程其实都很顺利，归功于文档实在是简单明了，整个过程像是看图写话，遇到的问题都可以换一个（无非是更麻烦的）别的方法来解决，所以在功能的实现上我没有遇到很大的问题（当然也有可能是因为我最后两步还没有完成，没有碰到最难的问题，不过我在log简单记录了一下huffman的思路，我觉得应该除工作量之外没有什么太大的问题）。
但除了功能，我后面花了很多时间尝试去做的是把代码封装成函数，或者说简化代码（至少是简化那些高度重复的部分），我遇到最大的问题也都集中在这里。我的代码中间的许多运算是针对数组进行的，所以我希望可以返回一个数组，可惜查了一下在cpp里行不通。所以我在继续查资料之后选择用memcpy和返回数组指针的方式进行尝试。也是在这个过程里，我感觉对指针这个概念的模糊让我在很多方面难以下手，所以我找了一些关于指针的介绍并仔细看了。
但无奈的是，当我按照理解的方式使用数组指针时，最后返回的时候总会出现一些报错，或者数据的值出错（和我之前写的未使用函数的代码产生的结果做的比较）。而memcpy也会有一些问题，很多时候我希望可以输入一个一维数组，输出一个二维数组，memcpy好像做不到这一点。
今天我查资料的时候(其实已经是下午，开始写readme了[ac01]）发现，可以直接把变换前和变换后的数组作为引用参数写到函数里，不需要写返回值，这样可以直接改变外部的数组。
现在想想或许可以用vector来实现这样的函数，因为返回值是可以是vector的，但ddl临近，我以后应该会去尝试一下。
中间我还遇到关于数据的储存的问题，我希望有一个下标可以用来标记数据，最开始比较简单，因为操作的都是数组，我只需要加一个维度就可以了。但后面当我需要储存游程编码的时候数组就不再适用了，所以我用vector进行储存。然后我说的下标问题就出现了，我当时没有不知道该怎么对n个vector添加index，又查了好多，选择用map来给vector添加index,但现在想想应该可以直接再套一个vector来操作下标，这应该会更好。

我的思考过程大概就是上面这些，我觉得在做这个面试题的过程中，我学到的东西里最重要的是封装的思想，虽然这个代码并不特别长，但把一些过程做成函数之后，不仅让界面更易懂简洁，也减少了思维的难度。但我本身在编程的过程中是先写完再进行封装的，我觉得这是我需要改进的一点，我应该多培养一些模块化的思想，可以从一开始就朝着制作函数的方向走，之后再写main。
其次我觉得我对一些概念的理解加深了（或从无到有），尤其是在写函数的过程中，包括查资料的过程中。虽然，要真正掌握这些基础的概念需要更系统的学习，但这个面试题给我做了一个初步的指引，我觉得这也是非常宝贵的。
但整个项目做下来我还是有很多限于技术能力的遗憾的，比如对游程编码那个过程的封装，我不知道该怎么做；比如huffman编码该怎么快速的导入（我写的思路是手动输入，但我想应该会有更好的方法）等等等等。
