#include <iostream>
#include <string>
#include<fstream>
#include <vector>
//#include<algorithm>
#include<math.h>
using namespace std;
//目前只差bne beq等等指令就正式完成！
//函數功能: 傳入一個字串s，以splitSep裡的字元當做分割字符，回傳vector<string>
vector<string> splitStr2Vec(string s, string splitSep)
{
	vector<string> result;
	int current = 0; //最初由 0 的位置開始找
	int next = 0;
	while (next != -1)
	{
		next = s.find_first_of(splitSep, current); //尋找從current開始，出現splitSep的第一個位置(找不到則回傳-1)
		if (next != current)
		{
			string tmp = s.substr(current, next - current);
			if (!tmp.empty())  //忽略空字串(若不寫的話，尾巴都是分割符會錯)
			{
				result.push_back(tmp);
			}
		}
		current = next + 1; //下次由 next + 1 的位置開始找起。
	}
	return result;
}
string inst_to_op(string inst[], int inst_lth); //inst轉乘opcode
string branch_to_op(string inst[], string Label[], int address,int label_amount);
string rg_to_string(string reg);
string imme_to_string(string imme);
string ujimme_to_string(string imme);
void find_label(string inst[][20], int address);
void execute(string inst[][20], int address);
void output(string pre_inst[], string inst[][20], int address);
int main()
{
	string input = "sw x9,240(x10)";
	fstream f("assembly.txt");
	fstream f2("assembly.txt");
	string inst[20][20];
	string str2 = "L";//用來確認是否有標籤
	string Label[100];
	int address = 0;  //紀錄address
	int inst_lth = 0; //instructure length
	int label_amount = 0;//紀錄指令行數

	/*while (getline(f, input)) {    //先把整組組語跑一次確定label位置
		inst_lth = 0;
		vector<string> sep = splitStr2Vec(input, " :,()");
		for (string t : sep) {
			inst[address/4][inst_lth] = t;
			//cout << t << endl;
			inst_lth++; //計算inst長度
		}
		//cout<<inst[1].size();
		if (inst[address / 4][0].find(str2) != string::npos) {
			Label[label_amount] = inst[address / 4][0] + to_string(address); //把Label名稱跟address存成一個string e.g. L2,address 8
			label_amount += 1;									//就存成"L28"
		}
		//cout << branch(inst);//還沒想要怎麼寫	
		address += 4;
		
	}*/
	address = 0;
	while (getline(f2, input)) {  //這組迴圈主要在轉換程式碼
		inst_lth = 1;
		inst[address / 4][0] = to_string(address);  //把address存在陣列第一位
		vector<string> sep = splitStr2Vec(input, " :,()");
		for (string t : sep) {
			if (t.find(str2) != string::npos && inst_lth == 0)//由於前面的label會影響計算，目前先用第一位是Label就continue												  
			{												////的方式不考慮他，因為address第一次的迴圈就有存起來了
				continue;                                   //也就是說讀到L2就跳過，讀L2後面的指令
			}  
			inst[address / 4][inst_lth] = t;
			//cout << inst[0] << endl;  //印出各字串
			inst_lth++; //計算inst長度
		}
		
		//cout<<inst[1].size();
		/*if(inst[0]=="beq"|| inst[0] == "bne"||inst[0] == "blt"
			||inst[0] == "bge"||inst[0] == "bltu"|| inst[0] == "bgeu")
		{
			cout << branch_to_op(inst, Label, address, label_amount) << endl;//還沒想要怎麼寫
		}
		else
		{
			cout << inst_to_op(inst, inst_lth) << endl;
		}*/
		for (int i = 0; i < inst_lth; i++)
		{
			cout << inst[address / 4][i] << " ";
		}
		cout << address<< endl;
		address += 4;
		
		
	}
	find_label(inst, address);
	
	system("pause");
}
void find_label(string inst[][20], int address)
{
	
	for (int i = 0; i < (address / 4); i++) {
		if (inst[i][1][0] == 'L' || inst[i][1][0] == 'E') {
			inst[i][9] = "Label";  //如果是label，將陣列第9項設為Label
			if (inst[i][2] == "beq"|| inst[i][2] == "bne") {
				inst[i][8] = "jump";
			}
		}
		else {
			if (inst[i][1] == "beq" || inst[i][2] == "bne") {
				inst[i][8] = "jump";
			}
		}
	}
	cout << inst[2][8] << " " << inst[2][9] << " " << inst[4][8] << endl; 
	execute(inst, address);
}
void execute(string inst[][20], int address) 
{
	int rg[20] = {};  //用陣列來記錄各個register的值  
	string pre_inst[20] = {};//存NTNT
	int i = 0;  //紀錄現在assembly code跑到哪！！！！
	while(i != (address / 4)-1){   //切記address會多1，因為程式是每行assembly都+4

		if (inst[i][1] == "addi"&&inst[i][3][1] == '0') {//確定是addi而且第二個register是R0，記得inst第一項是存address
			if (inst[i][9] != "Label") {							//記得inst第一項是存address
				rg[(int)(inst[i][2][1] - '0')] = stoi(inst[i][4]); //今天如果是R2透過此計算將後面的imme存到rg[2]
				i += 1;		//去下一行									   
				continue;
			}
		}
		if (inst[i][1] == "addi" && inst[i][3][1] != '0') {//確定是addi而且第二個register不是R0，那就是要計算
			if (inst[i][9] != "Label") {
				rg[(int)(inst[i][2][1] - '0')] = rg[(int)(inst[i][3][1] - '0')] + stoi(inst[i][4]); // 做addi的計算
				i += 1;  //去下一行
				continue;
			}
		}
		if (inst[i][1] == "add" && inst[i][3][1] != '0') {//add
			if (inst[i][9] != "Label") {
				rg[(int)(inst[i][2][1] - '0')] = rg[(int)(inst[i][3][1] - '0')] + rg[(int)(inst[i][4][1] - '0')]; //第二個rg+第三個rg
				i += 1;  //去下一行
				continue;
			}
		}
		if (inst[i][1] == "bne" && inst[i][8] == "jump") {
			if (inst[i][9] != "Label") {         //沒有Label的beq，通常是i++or i--之後回到迴圈起點
				if (rg[(int)(inst[i][2][1] - '0')] != rg[(int)(inst[i][3][1] - '0')]) {  //判斷2個register存的值一不一樣
					for (int j = 0; j < address / 4; j++) {     //一個一個inst確認要回到哪一個label
						if (inst[j][1] == inst[i][4])
						{
							pre_inst[i] += 'T';  //要在i被修改之前將NT值存進去
							i = stoi(inst[j][0]) / 4;  //找到了要回到的label，接著把i值改到那個地方的address
						}
					}

				}
				else {
					pre_inst[i] += 'N';
					i += 1;
					continue;
				}
			}
		}
		if (inst[i][1] == "beq" && inst[i][8] == "jump") {
			if (inst[i][9] != "Label") {         //沒有Label的beq，通常是i++or i--之後回到迴圈起點
				if (rg[(int)(inst[i][2][1] - '0')] == rg[(int)(inst[i][3][1] - '0')]) {  //判斷2個register存的值一不一樣
					for (int j = 0; j < address / 4; j++) {     //一個一個inst確認要回到哪一個label
						if (inst[j][1] == inst[i][4])
						{
							pre_inst[i] += 'T';  //要在i被修改之前將NT值存進去
							i = stoi(inst[j][0])/4;  //找到了要回到的label，接著把i值改到那個地方的address
						}
					}
					
				}
				else {
					pre_inst[i] += 'N';
					i += 1;
					continue;
				}
			}	
		}

		else {
			if (inst[i][9] == "Label") {
				if (rg[(int)(inst[i][3][1] - '0')] == rg[(int)(inst[i][4][1] - '0')]) {
					for (int j = 0; j < address / 4; j++) {
						if (inst[j][1] == inst[i][5])
						{
							pre_inst[i] += 'T';
							i = stoi(inst[j][0]) / 4;
						}
					}
					
				}
				else {
					pre_inst[i] += 'N';
					i += 1;
					continue;
				}
			}
		}
	}
	output(pre_inst, inst, address);
	cout << pre_inst[2] << endl;
	cout << pre_inst[4] << endl;
}
void output(string pre_inst[], string inst[][20], int address)
{
	int entry = 0;
	cout << "please insert the amount of entry you need:";
	cin >> entry;
	cout << endl;
	int place[20];//紀錄pre_inst所在位置的index
	int pre_amount = 0;  //紀錄有多少個predict inst.
	string taken[20][4];
	int misprediction[20] = {};
	for (int i = 0; i < 20; i++) {
		if (pre_inst[i].size() > 1) {
			place[pre_amount] = i; //只要偵測到有predict inst大於1就把index存起來
			pre_amount++;
			taken[i][0] = "SN";
			taken[i][1] = "SN";
			taken[i][2] = "SN";
			taken[i][3] = "SN";
		}
	}
	string state = "";  //紀錄predict時的00,01,11等等
	int history = 0; //記錄不同state要對到哪個2 bit history
	char predict = 'N'; //紀錄每次predict的值
	string temp = "NN";
	for (int i = 0; i < pre_amount; i++) {
		pre_inst[place[i]] = temp + pre_inst[place[i]];//此處注意！後來看老師範例output發現老師
													   //預測第一位時，前兩位預設為NN
	}
	for(int i=0;i<pre_amount;i++){ //看有幾組prediction inst就做幾次
		for (int j = 2; j < pre_inst[place[i]].size(); j++) { //place[i]會記錄下，要做branch的是第幾個inst
															  //並且，taken、pre_inst我都是把它紀錄在陣列第place[i]項
			if (pre_inst[place[i]][j - 2] == 'N'&&pre_inst[place[i]][j - 1] == 'N') { //判斷state及其對應的history
				state = "00";
				history = 0;
			}
			if (pre_inst[place[i]][j - 2] == 'N'&&pre_inst[place[i]][j - 1] == 'T') {//判斷state及其對應的history
				state = "01";
				history = 1;
			}
			if (pre_inst[place[i]][j - 2] == 'T'&&pre_inst[place[i]][j - 1] == 'N') {//判斷state及其對應的history
				state = "10";
				history = 2;
			}
			if (pre_inst[place[i]][j - 2] == 'T'&&pre_inst[place[i]][j - 1] == 'T') {//判斷state及其對應的history
				state = "11";
				history = 3;
			}
			if (state == "00"&&taken[place[i]][0]=="SN"|| state == "00"&&taken[place[i]][0] == "WN") //算出predict值
				predict = 'N';
			if (state == "00"&&taken[place[i]][0] == "ST" || state == "00"&&taken[place[i]][0] == "WT") //算出predict值
				predict = 'T';
			if (state == "01"&&taken[place[i]][1] == "SN" || state == "01"&&taken[place[i]][1] == "WN") //算出predict值
				predict = 'N';
			if (state == "01"&&taken[place[i]][1] == "ST" || state == "01"&&taken[place[i]][1] == "WT") //算出predict值
				predict = 'T';
			if (state == "10"&&taken[place[i]][2] == "SN" || state == "10"&&taken[place[i]][2] == "WN") //算出predict值
				predict = 'N';
			if (state == "10"&&taken[place[i]][2] == "ST" || state == "10"&&taken[place[i]][2] == "WT") //算出predict值
				predict = 'T';
			if (state == "11"&&taken[place[i]][3] == "SN" || state == "11"&&taken[place[i]][3] == "WN") //算出predict值
				predict = 'N';
			if (state == "11"&&taken[place[i]][3] == "ST" || state == "11"&&taken[place[i]][3] == "WT") //算出predict值
				predict = 'T';
			if (pre_inst[place[i]][j] != predict) {
				misprediction[place[i]] += 1;  //誤判累積數+1
				cout << "entry:" << (stoi(inst[place[i]][0]) / 4) % entry << "          ";  //address/4 mod entry
				for (int print = 1; print < 6; print++) {
					cout << inst[place[i]][print]<<" ";  //印出inst
				}
				cout << endl;
				cout << "(" <<state<<" "<< taken[place[i]][0] <<" "<< taken[place[i]][1] << " " << taken[place[i]][2] << " " << taken[place[i]][3]
					<< ") " << predict << " " << pre_inst[place[i]][j] << "  misprediction:" << misprediction[place[i]] << endl;;
				if (pre_inst[place[i]][j] == 'T'&&taken[place[i]][history] == "SN") { //判斷history需不需要變動
					taken[place[i]][history] = "WN";    
					continue;
				}
				if (pre_inst[place[i]][j] == 'T'&&taken[place[i]][history] == "WN") {//判斷history需不需要變動
					taken[place[i]][history] = "WT";
					continue;
				}
				if (pre_inst[place[i]][j] == 'N'&&taken[place[i]][history] == "WT") {//判斷history需不需要變動
					taken[place[i]][history] = "WN";
					continue;
				}
				if (pre_inst[place[i]][j] == 'N'&&taken[place[i]][history] == "ST") {//判斷history需不需要變動
					taken[place[i]][history] = "WT";
					continue;
				}
	
			}
			if (pre_inst[place[i]][j] == predict) {
				cout << "entry:" << (stoi(inst[place[i]][0]) / 4) % entry << "          ";
				for (int print = 1; print < 6; print++) {
					cout << inst[place[i]][print] << " ";
				}
				cout << endl;
				cout << "(" << state << " " << taken[place[i]][0] << " " << taken[place[i]][1] << " " << taken[place[i]][2] << " " << taken[place[i]][3]
					<< ") " << predict << " " << pre_inst[place[i]][j] << "  misprediction:" << misprediction[place[i]] << endl;
			}
		}
	}
	
	
	
	/*for (int i = 0; i < pre_amount; i++) {
		temp = pre_inst[place[i]].substr(pre_inst[place[i]].size() - 2, pre_inst[place[i]].size() - 1);  //取後面兩位(用來預測第一位)
		pre_inst[place[i]] = temp + pre_inst[place[i]];
	}*/
	/*for (int i = 0; i < address / 4; i++) {
		if (inst[i][1] != "beq") {
			cout << "entry:" << (stoi(inst[i][0]) / 4) % 4 << "        ";
			int j = 1;
			while (inst[i][j] != "") {
				cout << inst[i][j]<<" ";
				j++;
			}
			cout << endl << "No need to prediction" << endl;
		}
		if (inst[i][1] == "beq") {
			cout << "entry:" << (stoi(inst[i][0]) / 4) % 4;
			int j = 1;
			while (inst[i][j] != "") {
				cout << inst[i][j] << " ";
				j++;
			}
			
		}
	}*/
	//cout << pre_inst[place[0]] << endl << pre_inst[place[1]];
}
//string predict(string taken[][20], string inst[][20], string pre_inst[], int pre_amount, int place[], int i) {

//}
string branch_to_op(string inst[],string Label[],int address,int label_amount)
{
	string rs2 = "none";
	string rs1 = "none";
	string imme = "none";
	string opcode = "1100011";
	string func3 = "none";
	string final = "none";
	if (inst[0] == "beq")func3 = "000";
	if (inst[0] == "bne")func3 = "001";
	if (inst[0] == "blt")func3 = "100";
	if (inst[0] == "bltu")func3 = "110";
	if (inst[0] == "bgeu")func3 = "111";
	int distance = 0;
	for (int i = 0; i < label_amount; i++) {
		if (Label[i].substr(0, 2) == inst[3]) {
			distance = stoi(Label[i].substr(2, Label[i].size() - 2)) - address; //address就是我目前的位址
			break;
		}
	}
	imme = imme_to_string(to_string(distance));
	imme = imme[0] + imme; //是負數就+1是正數就+0
	rs2 = rg_to_string(inst[2]);
	rs1 = rg_to_string(inst[1]);
	final = imme[0] + imme.substr(2, 6) + rs2 + rs1 + func3 + imme.substr(8, 4) + imme[1] + opcode;
	//cout << imme << endl;
	return final;
}
string inst_to_op(string inst[], int inst_lth) 
{
	//cout << inst[0];
	string type = "none";
	string opcode = "none";
	string func5 = "none";
	string func3 = "none";
	string func2 = "none";
	string rs2 = "none";
	string rd = "none";
	string rs1 = "none";
	string imme = "none";
	string final = "none";
	if (inst[0] == "add" || inst[0] == "sub" || inst[0] == "sll" || inst[0] == "slt" //type = R
		|| inst[0] == "sltu" || inst[0] == "xor" || inst[0] == "srl" || inst[0] == "sra" || inst[0] == "or" || inst[0] == "and") 
	{
		if (inst[0] == "add" || inst[0] == "sub") func3 = "000";
		if (inst[0] == "sll") func3 = "001";
		if (inst[0] == "slt") func3 = "010";
		if (inst[0] == "sltu") func3 = "011";
		if (inst[0] == "xor") func3 = "100";
		if (inst[0] == "srl" || inst[0] == "sra") func3 = "101";
		if (inst[0] == "or") func3 = "110";
		if (inst[0] == "and") func3 = "111";
		type = "R";
		opcode = "0110011";
		if (inst[0] == "sub" || inst[0] == "sra") {
			func5 = "01000";
			func2 = "00";
		}
		else {
			func5 = "00000";
			func2 = "00";
		}
		rd = rg_to_string(inst[1]); //把reg轉成二進制字串
		rs1 = rg_to_string(inst[2]);
		rs2 = rg_to_string(inst[3]);
		final = func5 + func2 + rs2 + rs1 + func3 + rd + opcode;
	}

	if (inst[0] == "addi" || inst[0] == "slti" //type = I,不包含load，記得要處理負數情況
		|| inst[0] == "sltiu" || inst[0] == "xori" || inst[0] == "ori" || inst[0] == "andi") 
	{
		if (inst[0] == "addi") func3 = "000";
		if (inst[0] == "slti") func3 = "010";
		if (inst[0] == "sltiu") func3 = "011";
		if (inst[0] == "xori") func3 = "100";
		if (inst[0] == "ori") func3 = "110";
		if (inst[0] == "andi") func3 = "111";
		type = "I";
		opcode = "0010011";
		rd = rg_to_string(inst[1]); //把reg轉成二進制字串
		rs1 = rg_to_string(inst[2]);
		imme = imme_to_string(inst[3]);
		final = imme + rs1 + func3 + rd + opcode;
	}
	if (inst[0] == "lb" || inst[0] == "lh" //load系列
		|| inst[0] == "lw" || inst[0] == "lbu" || inst[0] == "lhu") 
	{
		if (inst[0] == "lb") func3 = "000";
		if (inst[0] == "lw") func3 = "010";
		if (inst[0] == "lh") func3 = "001";
		if (inst[0] == "lbu") func3 = "100";
		if (inst[0] == "lhu") func3 = "101";
		type = "I";
		opcode = "0000011";
		rd = rg_to_string(inst[1]);
		imme = imme_to_string(inst[2]);
		rs1 = rg_to_string(inst[3]);
		final = imme + rs1 + func3 + rd + opcode;
	}
	if (inst[0] == "sb" || inst[0] == "sh" //s type
		|| inst[0] == "sw")
	{
		if (inst[0] == "sb") func3 = "000";
		if (inst[0] == "sw") func3 = "010";
		if (inst[0] == "sh") func3 = "001";
		type = "S";
		opcode = "0100011";
		rs2 = rg_to_string(inst[1]);
		imme = imme_to_string(inst[2]);
		rs1 = rg_to_string(inst[3]);
		final = imme.substr(0,7) + rs2 +rs1 + func3 + imme.substr(7,5) + opcode; //用substr去擷取imme
	}
	if (inst[0] == "jal")
	{
		type = "UJ";
		opcode = "1101111";
		rd = rg_to_string(inst[1]);
		imme = ujimme_to_string(inst[2]);
		//cout << imme << endl;
		final = imme[0] + imme.substr(10, 10) + imme[9] + imme.substr(1, 8) + rd + opcode;
	}
	if (inst[0] == "jalr")   //格式跟Load非常像
	{
		type = "UJ";
		opcode = "1100111";
		func3 = "000";
		rd = rg_to_string(inst[1]);
		imme = imme_to_string(inst[2]);
		rs1 = rg_to_string(inst[3]);
		//cout << imme << endl;
		final = imme + rs1 + func3 + rd + opcode;
	}
	return final;
}
string ujimme_to_string(string imme) 
{
	int count = 0;
	int dcnum = 0;
	string num = ""; //儲存register數字部分
	dcnum = stoi(imme);
	for (int i = 0; i < 20; i++) {
		num += to_string(dcnum % 2);
		dcnum /= 2;
		//if (dcnum > 0) { count += 1; }
	}
	reverse(num.begin(), num.end());
	num = '0' + num;
	return num;
}
string imme_to_string(string imme)
{
	int count = 0;
	int dcnum = 0;
	string num = ""; //儲存register數字部分
	if (imme[0] != '-') {  //如果是正數直接算就好
		//num = imme;
		dcnum = stoi(imme);
		for (int i = 0; i < 12; i++) {
			num += to_string(dcnum % 2);
			dcnum /= 2;
			//if (dcnum > 0) { count += 1; }
		}
		reverse(num.begin(), num.end());
		//cout << count << endl;
	}
	if (imme[0] == '-') {  //現在要想怎麼處理二補數
		for (int i = 1; i < imme.size(); i++) {
			num += (imme[i]);
		}
		dcnum = stoi(num);
		string temp = "";
		num = "";
		while(dcnum > 0){       //如果是-9就先算出9
			temp += to_string(dcnum % 2);
			dcnum /= 2;
			//count += 1;    //1001會得到count=4      
		}
		reverse(temp.begin(), temp.end());
		for (int i = 0; i < temp.size(); i++) {  //flip
			if (temp[i] == '0')
				temp[i] = '1';
			else
				temp[i] = '0';
		}
		char add = '1';//補數值+1&進位值
		for (int i = temp.size()-1; i >= 0; i--){ //flip完+1
			
			if (temp[i] == '1' && add == '1') {
				temp[i] = '0';
				add = '1';
				continue; //只要做一次運算就好，不用continue的話，符合下面條件又會再計算一次
			}
			if (temp[i] == '1' && add == '0') {
				temp[i] = '1';
				add = '0';
				continue; //只要做一次運算就好，不用continue的話，符合下面條件又會再計算一次
			}
			if (temp[i] == '0' && add == '1') {
				temp[i] = '1';
				add = '0';
				continue;//只要做一次運算就好，不用continue的話，符合下面條件又會再計算一次
			}
			if (temp[i] == '0' && add == '0') {
				temp[i] = '0';
				add = '0';
				continue;//只要做一次運算就好，不用continue的話，符合下面條件又會再計算一次
			}
		}
		num = temp;
		while (num.size() < 12) {
			num = '1' + num;
		}
		//cout << num << endl;
	}
	return num;
}

string rg_to_string(string reg) 
{
	int dcnum = 0;
	string num =""; //儲存register數字部分
	for (int i = 1; i < reg.size(); i++) {
		num += (reg[i]);
	}
	dcnum = stoi(num);
	num = "";
	for (int i = 0; i < 5; i++) {
		num += to_string(dcnum % 2);
		dcnum /= 2;
	}
	reverse(num.begin(), num.end());
	return num;
}