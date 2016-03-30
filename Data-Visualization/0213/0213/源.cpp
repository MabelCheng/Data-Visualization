#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>

using namespace std;

/*�㷨�Ķ���ѡ��ʱ�临�Ӷ�����*/

class Cnode{
public:
	void set_fnode(vector<string> s){ fnode.push_back(s); }
	void set_snode(vector<string> s){ snode.push_back(s); }
	void set_weight(double d){ weight = d; }
	vector<vector<string>> get_fnode(){ return fnode; }
	vector<vector<string>> get_snode(){ return snode; }
	double get_weight(){ return weight; }
	void clear(){ fnode.clear(); snode.clear(); weight = 0.0; }

private:
	vector<vector<string>> fnode;
	vector<vector<string>> snode;
	double weight = 0.0;
}cnode;

vector<vector<string>>name(1), name2(1);
vector<vector<string>> nametemp, nametemp2;
vector<Cnode> vcnode, vcnodetemp;
double distance_result = 1000000;
double distance_temp;
double ratio_result = 1;
vector<double> weight_compress;

//show
void show(vector<Cnode> vc){
	ofstream ofile;
	ofile.open("d:\\4.json");

	for (size_t i = 0; i < vc.size(); i++)
	{
		vector<vector<string>> fn;
		vector<vector<string>> sn;
		double w;
		fn = vc[i].get_fnode();
		sn = vc[i].get_snode();
		w = vc[i].get_weight();
		ofile << "first : ";
		for (size_t j = 0; j < fn.size(); j++)
		{
			for (size_t m = 0; m < fn[j].size(); m++)
			{
				ofile << fn[j][m] << " ";
			}
		}
		ofile << "second: ";
		for (size_t j = 0; j < sn.size(); j++)
		{
			for (size_t m = 0; m < sn[j].size(); m++)
			{
				ofile << sn[j][m] << " ";
			}
		}
		ofile << " weight : " << w;
		ofile << "\n";
	}
	ofile.close();

}
//to add last part to temp vector<Cnode>
vector<Cnode> add_(vector<Cnode> vc, vector<Cnode> vc1)
{
	vector<Cnode>::iterator iter = vc.begin();
	for (; iter != vc.end(); iter++)
	{
		vc1.push_back(*iter);
	}
	return vc1;
}

//to return the distance between a pair of nodes
double single_distance(double d, vector<double> vd)
{
	double dd = 0;
	for (size_t i = 0; i < vd.size(); i++)
	{
		dd += (d - vd[i])*(d - vd[i]);
	}
	return dd;
}

//to judge whether the num of weight is enough, yes:return 0 ;no return num
vector<double> isenough(vector<double> vd, vector<vector<string>> vs, vector<vector<string>>vs1)
{
	int max = 0;
	if (vs == vs1)
		max = vs[0].size()*(vs[0].size() + 1) / 2;
	else if (vs[0].size() >= vs1[0].size())
		max = vs[0].size();
	if (vd.size() < max)
	{
		vd.insert(vd.end(), (max - vd.size()), 0);
	}
	return vd;
}


//to change string to number
template <class Type>
Type StrToNum(const string& str){
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

//to judge if a string is a number
bool isnum(string s){
	stringstream instr(s);
	double d;
	char c;
	if (!(instr >> d))
		return false;
	if (instr >> c)
		return false;
	else
		return true;
}

//to judge whether the name is exist in the vector, if yes return the index, else return -1
int isexistvec(string s, vector<vector<string>> vc){
	for (size_t i = 0; i < vc.size(); i++)
	{
		for (size_t j = 0; j < vc[i].size(); j++)
		{
			if (!s.compare(vc[i][j]))
				return 0;
		}
	}
	return -1;
}
int isexist(string s, vector<string> vs)
{
	for (size_t i = 0; i < vs.size(); i++)
	{
		if (!s.compare(vs[i]))
			return 0;
	}
}

int isequal(vector<vector<string>> vs, vector<string>vc){
	for (size_t i = 0; i < vs.size(); i++)
	{
		for (size_t j = 0; j < vs[i].size(); j++)
		{
			for (size_t k = 0; k < vc.size(); k++)
			{
				if (!vs[i][j].compare(vc[k]))
					return 0;
			}
		}
	}
	return -1;
}

//to measure the compression ratio  number of edges(new/old)
double get_ratio(vector<Cnode> newone, double oldsize)
{
	return (double)newone.size() / oldsize;
}

void main(){
	double ratio;
	cout << "please enter the ratio tou want to measure:";
	cin >> ratio;
	string filename("d://1.txt");
	ifstream fin(filename.c_str());

	if (!fin)
		cout << "File open in error!\n";

	string toread;
	int ii = 0;//����name��һ��vectorֻ��һ��Ԫ��
	while (fin >> toread){
		if (toread == "http://biomine.cs.helsinki.fi/bm/links.cgi")
		{
			int flag = 0;
			string temp;
			string temp_s;//������ʱ���node��

			while (fin >> toread)
			{
				if (toread != "#")
				{
					if (!isnum(toread))
					{
						if (flag == 0)
						{
							vector<string> temp;
							temp.push_back(toread);
							cnode.set_fnode(temp);
							flag++;
						}
						else
						{
							vector<string> temp;
							temp.push_back(toread);
							cnode.set_snode(temp);
							flag = 0;
							//iter++;
						}
						if (isexistvec(toread, name) < 0)
						{
							name[ii].push_back(toread);
				//			nametemp.push_back(toread);
							ii++;
							name.resize(ii + 1);
						}

					}
					else{
						cnode.set_weight(StrToNum<double>(toread));
						vcnode.push_back(cnode);
						cnode.clear();
					}
				}
				else
					break;
			}
		}
	}//ע������������ж�
	//��vcnode �Լ�name
	fin.close();
	vcnodetemp = vcnode;//ÿ���һ��ѹ���������¸�ֵһ��

	//��ʼѹ���Ĺ���
	double weight_temp = 0;
	double weight_result = 0;
	vector<vector<string>> name_temp(1);//ÿһ��ѹ��֮��Ľ��������
	vector<vector<string>> name_result(1);//���֤��ѹ�����
	vector<Cnode> vcnode_temp;//ÿһ��ѹ��֮���ͼ��״̬
	vector<Cnode> vcnode_result;//��󱣴��ѹ��ͼ

	vcnode_result = vcnode;
	double oldsize = vcnode.size();
	name.resize(ii);//֮ǰ�����1 ������һ���ǿյģ���ʱ������ԭ���Է�֮��������⡣
	name2 = name;

	while (ratio_result > ratio)
	{
		vcnode = vcnode_result;
		name = name2;
		distance_result = 100000;
		for (size_t i = 0; i < name.size() - 1; i++)
		{
			size_t j = i + 1;
			for (; j < name.size(); j++)
			{
				//ѡ����Ҫ�ϲ�������node���������µ�namevector���д洢��
				/*
				���ȼ���self-node��weight
				����Ϊ���򴴽��µ�cnode �Լ�vcnode���д洢����*/

				//�˴�������- -����ж��Ƿ��������һ�ֱ���֮��ĸ�ֵ
				vcnodetemp = vcnode;
				nametemp2 = name;//ÿ���һ��ѹ���������¸�ֵһ��
				vector<Cnode> ::iterator itertemp = vcnodetemp.begin();


				//��Ҫ�ϲ��Ľ����ܳ��µĽ�㣬��ʱӦ����
				name_temp[0].insert(name_temp[0].end(), name[i].begin(), name[i].end());
				name_temp[0].insert(name_temp[0].end(), name[j].begin(), name[j].end());

				while (itertemp != vcnodetemp.end())
				{
					if (isequal(itertemp->get_fnode(), name[i]) == 0 || isequal(itertemp->get_fnode(), name[j]) == 0)//˵������ Ҫ���ҵ�name ���ڲ���ǰ�����Ա���˫���ж�
					{
						if (isequal(itertemp->get_snode(), name[j]) == 0 || isequal(itertemp->get_snode(), name[i]) == 0)
						{
							weight_temp += itertemp->get_weight();
							//�˴���Ҫ��¼ԭ����weight�����ٴα�����������һ��vector�Ϳ���ֱ��дһ����������������Ϊ���ܴ����޹����ߵ�����������漰����������⡣
							weight_compress.push_back(itertemp->get_weight());
							itertemp = vcnodetemp.erase(itertemp);
						}
						else
							itertemp++;
					}
					else
						itertemp++;
				}

				if (weight_temp != 0)
				{
					Cnode cnode_t;
					cnode_t.set_fnode(name_temp[0]);
					cnode_t.set_snode(name_temp[0]);
					cnode_t.set_weight(weight_temp);
					vcnode_temp.push_back(cnode_t);
					//�ж��ĸ������ӽ�����Ƚ϶࣬���㲹����ͬ�ĸ�����
					weight_compress = isenough(weight_compress, cnode_t.get_fnode(), cnode_t.get_snode());
					distance_temp = single_distance(weight_temp, weight_compress);
					if (distance_temp > distance_result)
						goto loop;
					weight_temp = 0;
					weight_compress.clear();
				}

				//�˴�ɾ���Ѿ��ϲ��Ľ�㣬����֮����б���
				for (size_t i = 0; i < name_temp[0].size(); i++)
				{
					for (vector<vector<string>>::iterator iter_name = nametemp2.begin(); iter_name != nametemp2.end();)
					{
						for (size_t j = 0; j < (*iter_name).size(); j++)
						{
							if (name_temp[0][i] == (*iter_name)[j])
							{
								iter_name = nametemp2.erase(iter_name);
								break;
							}
							else
							{
								iter_name++;
								break;
							}
								
						}
						
					}
				}
				//��ϳ��µ�name �� ��һ�ַ����Ѿ�����
				name_temp.insert(name_temp.end(), nametemp2.begin(), nametemp2.end());

				for (size_t k = 0; k < nametemp2.size(); k++)
				{
					vector<Cnode>::iterator iter = vcnodetemp.begin();
					while (iter != vcnodetemp.end())
					{
						if (isequal(iter->get_fnode(), name_temp[0]) == 0 || isequal(iter->get_snode(), name_temp[0]) == 0)
						{
							if (isequal(iter->get_fnode(), nametemp2[k]) == 0 || isequal(iter->get_snode(), nametemp2[k]) == 0)
							{
								weight_temp += iter->get_weight();
								weight_compress.push_back(iter->get_weight());
								iter = vcnodetemp.erase(iter);
							}
							else
								iter++;
						}
						else
							iter++;
					}
					if (weight_temp != 0)
					{
						Cnode cnode_t;
						cnode_t.set_fnode(name_temp[0]);
						cnode_t.set_snode(nametemp2[k]);
						weight_temp = weight_temp / cnode_t.get_fnode()[0].size() / cnode_t.get_snode()[0].size();
						cnode_t.set_weight(weight_temp);
						vcnode_temp.push_back(cnode_t);
						weight_compress = isenough(weight_compress, cnode_t.get_fnode(), cnode_t.get_snode());
						distance_temp = distance_temp + single_distance(weight_temp, weight_compress);
						if (distance_temp > distance_result)
							goto loop;
						weight_temp = 0;
						weight_compress.clear();
					}
				}
				vcnode_temp = add_(vcnodetemp, vcnode_temp);//��ʣ�µ����������		
				vcnode_result = vcnode_temp;//��Ϊ��һ�ֽ������
				distance_result = distance_temp;//��Ϊ��һ�ֽ������
				name2 = name_temp;//��Ϊ��һ�� name�Ľ�����档
			loop:
				vcnode_temp.clear();
				weight_compress.clear();
				weight_temp = 0;
				distance_temp = 0;
				name_temp.clear();
				name_temp.resize(1);
			}						
		}
		ratio_result = get_ratio(vcnode_result, oldsize);
		show(vcnode_result);
		cout << ratio_result << "\n";
		cout << distance_result;
		}
		

		show(vcnode_result);
}
