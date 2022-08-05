#include <iostream>
#include <string>
#include<fstream>
#include <vector>
//#include<algorithm>
#include<math.h>
using namespace std;
//�ثe�u�tbne beq�������O�N���������I
//��ƥ\��: �ǤJ�@�Ӧr��s�A�HsplitSep�̪��r�������Φr�šA�^��vector<string>
vector<string> splitStr2Vec(string s, string splitSep)
{
	vector<string> result;
	int current = 0; //�̪�� 0 ����m�}�l��
	int next = 0;
	while (next != -1)
	{
		next = s.find_first_of(splitSep, current); //�M��qcurrent�}�l�A�X�{splitSep���Ĥ@�Ӧ�m(�䤣��h�^��-1)
		if (next != current)
		{
			string tmp = s.substr(current, next - current);
			if (!tmp.empty())  //�����Ŧr��(�Y���g���ܡA���ڳ��O���βŷ|��)
			{
				result.push_back(tmp);
			}
		}
		current = next + 1; //�U���� next + 1 ����m�}�l��_�C
	}
	return result;
}
string inst_to_op(string inst[], int inst_lth); //inst�୼opcode
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
	string str2 = "L";//�ΨӽT�{�O�_������
	string Label[100];
	int address = 0;  //����address
	int inst_lth = 0; //instructure length
	int label_amount = 0;//�������O���

	/*while (getline(f, input)) {    //�����ղջy�]�@���T�wlabel��m
		inst_lth = 0;
		vector<string> sep = splitStr2Vec(input, " :,()");
		for (string t : sep) {
			inst[address/4][inst_lth] = t;
			//cout << t << endl;
			inst_lth++; //�p��inst����
		}
		//cout<<inst[1].size();
		if (inst[address / 4][0].find(str2) != string::npos) {
			Label[label_amount] = inst[address / 4][0] + to_string(address); //��Label�W�ٸ�address�s���@��string e.g. L2,address 8
			label_amount += 1;									//�N�s��"L28"
		}
		//cout << branch(inst);//�٨S�Q�n���g	
		address += 4;
		
	}*/
	address = 0;
	while (getline(f2, input)) {  //�o�հj��D�n�b�ഫ�{���X
		inst_lth = 1;
		inst[address / 4][0] = to_string(address);  //��address�s�b�}�C�Ĥ@��
		vector<string> sep = splitStr2Vec(input, " :,()");
		for (string t : sep) {
			if (t.find(str2) != string::npos && inst_lth == 0)//�ѩ�e����label�|�v�T�p��A�ثe���βĤ@��OLabel�Ncontinue												  
			{												////���覡���Ҽ{�L�A�]��address�Ĥ@�����j��N���s�_�ӤF
				continue;                                   //�]�N�O��Ū��L2�N���L�AŪL2�᭱�����O
			}  
			inst[address / 4][inst_lth] = t;
			//cout << inst[0] << endl;  //�L�X�U�r��
			inst_lth++; //�p��inst����
		}
		
		//cout<<inst[1].size();
		/*if(inst[0]=="beq"|| inst[0] == "bne"||inst[0] == "blt"
			||inst[0] == "bge"||inst[0] == "bltu"|| inst[0] == "bgeu")
		{
			cout << branch_to_op(inst, Label, address, label_amount) << endl;//�٨S�Q�n���g
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
			inst[i][9] = "Label";  //�p�G�Olabel�A�N�}�C��9���]��Label
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
	int rg[20] = {};  //�ΰ}�C�ӰO���U��register����  
	string pre_inst[20] = {};//�sNTNT
	int i = 0;  //�����{�bassembly code�]����I�I�I�I
	while(i != (address / 4)-1){   //���Oaddress�|�h1�A�]���{���O�C��assembly��+4

		if (inst[i][1] == "addi"&&inst[i][3][1] == '0') {//�T�w�Oaddi�ӥB�ĤG��register�OR0�A�O�oinst�Ĥ@���O�saddress
			if (inst[i][9] != "Label") {							//�O�oinst�Ĥ@���O�saddress
				rg[(int)(inst[i][2][1] - '0')] = stoi(inst[i][4]); //���Ѧp�G�OR2�z�L���p��N�᭱��imme�s��rg[2]
				i += 1;		//�h�U�@��									   
				continue;
			}
		}
		if (inst[i][1] == "addi" && inst[i][3][1] != '0') {//�T�w�Oaddi�ӥB�ĤG��register���OR0�A���N�O�n�p��
			if (inst[i][9] != "Label") {
				rg[(int)(inst[i][2][1] - '0')] = rg[(int)(inst[i][3][1] - '0')] + stoi(inst[i][4]); // ��addi���p��
				i += 1;  //�h�U�@��
				continue;
			}
		}
		if (inst[i][1] == "add" && inst[i][3][1] != '0') {//add
			if (inst[i][9] != "Label") {
				rg[(int)(inst[i][2][1] - '0')] = rg[(int)(inst[i][3][1] - '0')] + rg[(int)(inst[i][4][1] - '0')]; //�ĤG��rg+�ĤT��rg
				i += 1;  //�h�U�@��
				continue;
			}
		}
		if (inst[i][1] == "bne" && inst[i][8] == "jump") {
			if (inst[i][9] != "Label") {         //�S��Label��beq�A�q�`�Oi++or i--����^��j��_�I
				if (rg[(int)(inst[i][2][1] - '0')] != rg[(int)(inst[i][3][1] - '0')]) {  //�P�_2��register�s���Ȥ@���@��
					for (int j = 0; j < address / 4; j++) {     //�@�Ӥ@��inst�T�{�n�^����@��label
						if (inst[j][1] == inst[i][4])
						{
							pre_inst[i] += 'T';  //�n�bi�Q�ק蠟�e�NNT�Ȧs�i�h
							i = stoi(inst[j][0]) / 4;  //���F�n�^�쪺label�A���ۧ�i�ȧ�쨺�Ӧa�誺address
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
			if (inst[i][9] != "Label") {         //�S��Label��beq�A�q�`�Oi++or i--����^��j��_�I
				if (rg[(int)(inst[i][2][1] - '0')] == rg[(int)(inst[i][3][1] - '0')]) {  //�P�_2��register�s���Ȥ@���@��
					for (int j = 0; j < address / 4; j++) {     //�@�Ӥ@��inst�T�{�n�^����@��label
						if (inst[j][1] == inst[i][4])
						{
							pre_inst[i] += 'T';  //�n�bi�Q�ק蠟�e�NNT�Ȧs�i�h
							i = stoi(inst[j][0])/4;  //���F�n�^�쪺label�A���ۧ�i�ȧ�쨺�Ӧa�誺address
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
	int place[20];//����pre_inst�Ҧb��m��index
	int pre_amount = 0;  //�������h�֭�predict inst.
	string taken[20][4];
	int misprediction[20] = {};
	for (int i = 0; i < 20; i++) {
		if (pre_inst[i].size() > 1) {
			place[pre_amount] = i; //�u�n�����즳predict inst�j��1�N��index�s�_��
			pre_amount++;
			taken[i][0] = "SN";
			taken[i][1] = "SN";
			taken[i][2] = "SN";
			taken[i][3] = "SN";
		}
	}
	string state = "";  //����predict�ɪ�00,01,11����
	int history = 0; //�O�����Pstate�n������2 bit history
	char predict = 'N'; //�����C��predict����
	string temp = "NN";
	for (int i = 0; i < pre_amount; i++) {
		pre_inst[place[i]] = temp + pre_inst[place[i]];//���B�`�N�I��ӬݦѮv�d��output�o�{�Ѯv
													   //�w���Ĥ@��ɡA�e���w�]��NN
	}
	for(int i=0;i<pre_amount;i++){ //�ݦ��X��prediction inst�N���X��
		for (int j = 2; j < pre_inst[place[i]].size(); j++) { //place[i]�|�O���U�A�n��branch���O�ĴX��inst
															  //�åB�Ataken�Bpre_inst�ڳ��O�⥦�����b�}�C��place[i]��
			if (pre_inst[place[i]][j - 2] == 'N'&&pre_inst[place[i]][j - 1] == 'N') { //�P�_state�Ψ������history
				state = "00";
				history = 0;
			}
			if (pre_inst[place[i]][j - 2] == 'N'&&pre_inst[place[i]][j - 1] == 'T') {//�P�_state�Ψ������history
				state = "01";
				history = 1;
			}
			if (pre_inst[place[i]][j - 2] == 'T'&&pre_inst[place[i]][j - 1] == 'N') {//�P�_state�Ψ������history
				state = "10";
				history = 2;
			}
			if (pre_inst[place[i]][j - 2] == 'T'&&pre_inst[place[i]][j - 1] == 'T') {//�P�_state�Ψ������history
				state = "11";
				history = 3;
			}
			if (state == "00"&&taken[place[i]][0]=="SN"|| state == "00"&&taken[place[i]][0] == "WN") //��Xpredict��
				predict = 'N';
			if (state == "00"&&taken[place[i]][0] == "ST" || state == "00"&&taken[place[i]][0] == "WT") //��Xpredict��
				predict = 'T';
			if (state == "01"&&taken[place[i]][1] == "SN" || state == "01"&&taken[place[i]][1] == "WN") //��Xpredict��
				predict = 'N';
			if (state == "01"&&taken[place[i]][1] == "ST" || state == "01"&&taken[place[i]][1] == "WT") //��Xpredict��
				predict = 'T';
			if (state == "10"&&taken[place[i]][2] == "SN" || state == "10"&&taken[place[i]][2] == "WN") //��Xpredict��
				predict = 'N';
			if (state == "10"&&taken[place[i]][2] == "ST" || state == "10"&&taken[place[i]][2] == "WT") //��Xpredict��
				predict = 'T';
			if (state == "11"&&taken[place[i]][3] == "SN" || state == "11"&&taken[place[i]][3] == "WN") //��Xpredict��
				predict = 'N';
			if (state == "11"&&taken[place[i]][3] == "ST" || state == "11"&&taken[place[i]][3] == "WT") //��Xpredict��
				predict = 'T';
			if (pre_inst[place[i]][j] != predict) {
				misprediction[place[i]] += 1;  //�~�P�ֿn��+1
				cout << "entry:" << (stoi(inst[place[i]][0]) / 4) % entry << "          ";  //address/4 mod entry
				for (int print = 1; print < 6; print++) {
					cout << inst[place[i]][print]<<" ";  //�L�Xinst
				}
				cout << endl;
				cout << "(" <<state<<" "<< taken[place[i]][0] <<" "<< taken[place[i]][1] << " " << taken[place[i]][2] << " " << taken[place[i]][3]
					<< ") " << predict << " " << pre_inst[place[i]][j] << "  misprediction:" << misprediction[place[i]] << endl;;
				if (pre_inst[place[i]][j] == 'T'&&taken[place[i]][history] == "SN") { //�P�_history�ݤ��ݭn�ܰ�
					taken[place[i]][history] = "WN";    
					continue;
				}
				if (pre_inst[place[i]][j] == 'T'&&taken[place[i]][history] == "WN") {//�P�_history�ݤ��ݭn�ܰ�
					taken[place[i]][history] = "WT";
					continue;
				}
				if (pre_inst[place[i]][j] == 'N'&&taken[place[i]][history] == "WT") {//�P�_history�ݤ��ݭn�ܰ�
					taken[place[i]][history] = "WN";
					continue;
				}
				if (pre_inst[place[i]][j] == 'N'&&taken[place[i]][history] == "ST") {//�P�_history�ݤ��ݭn�ܰ�
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
		temp = pre_inst[place[i]].substr(pre_inst[place[i]].size() - 2, pre_inst[place[i]].size() - 1);  //���᭱���(�Ψӹw���Ĥ@��)
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
			distance = stoi(Label[i].substr(2, Label[i].size() - 2)) - address; //address�N�O�ڥثe����}
			break;
		}
	}
	imme = imme_to_string(to_string(distance));
	imme = imme[0] + imme; //�O�t�ƴN+1�O���ƴN+0
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
		rd = rg_to_string(inst[1]); //��reg�ন�G�i��r��
		rs1 = rg_to_string(inst[2]);
		rs2 = rg_to_string(inst[3]);
		final = func5 + func2 + rs2 + rs1 + func3 + rd + opcode;
	}

	if (inst[0] == "addi" || inst[0] == "slti" //type = I,���]�tload�A�O�o�n�B�z�t�Ʊ��p
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
		rd = rg_to_string(inst[1]); //��reg�ন�G�i��r��
		rs1 = rg_to_string(inst[2]);
		imme = imme_to_string(inst[3]);
		final = imme + rs1 + func3 + rd + opcode;
	}
	if (inst[0] == "lb" || inst[0] == "lh" //load�t�C
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
		final = imme.substr(0,7) + rs2 +rs1 + func3 + imme.substr(7,5) + opcode; //��substr�h�^��imme
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
	if (inst[0] == "jalr")   //�榡��Load�D�`��
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
	string num = ""; //�x�sregister�Ʀr����
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
	string num = ""; //�x�sregister�Ʀr����
	if (imme[0] != '-') {  //�p�G�O���ƪ�����N�n
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
	if (imme[0] == '-') {  //�{�b�n�Q���B�z�G�ɼ�
		for (int i = 1; i < imme.size(); i++) {
			num += (imme[i]);
		}
		dcnum = stoi(num);
		string temp = "";
		num = "";
		while(dcnum > 0){       //�p�G�O-9�N����X9
			temp += to_string(dcnum % 2);
			dcnum /= 2;
			//count += 1;    //1001�|�o��count=4      
		}
		reverse(temp.begin(), temp.end());
		for (int i = 0; i < temp.size(); i++) {  //flip
			if (temp[i] == '0')
				temp[i] = '1';
			else
				temp[i] = '0';
		}
		char add = '1';//�ɼƭ�+1&�i���
		for (int i = temp.size()-1; i >= 0; i--){ //flip��+1
			
			if (temp[i] == '1' && add == '1') {
				temp[i] = '0';
				add = '1';
				continue; //�u�n���@���B��N�n�A����continue���ܡA�ŦX�U������S�|�A�p��@��
			}
			if (temp[i] == '1' && add == '0') {
				temp[i] = '1';
				add = '0';
				continue; //�u�n���@���B��N�n�A����continue���ܡA�ŦX�U������S�|�A�p��@��
			}
			if (temp[i] == '0' && add == '1') {
				temp[i] = '1';
				add = '0';
				continue;//�u�n���@���B��N�n�A����continue���ܡA�ŦX�U������S�|�A�p��@��
			}
			if (temp[i] == '0' && add == '0') {
				temp[i] = '0';
				add = '0';
				continue;//�u�n���@���B��N�n�A����continue���ܡA�ŦX�U������S�|�A�p��@��
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
	string num =""; //�x�sregister�Ʀr����
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