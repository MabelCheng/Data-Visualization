#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<map>
#include<list>

using namespace std;

/*算法的多项选择，时间复杂度问题*/

class TreeNode{
private:
	vector<string> _data;
	TreeNode *parent;
	TreeNode *childleft;
	TreeNode *childright;

public:
	void setParent(TreeNode *p){ parent = p; };
	void setChildl(TreeNode *c){ childleft = c; };
	void setChildr(TreeNode *c){ childright = c; };
	void setData(vector<string> d){ _data = d; };
};
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
//用于存放压缩的过程，map那货居然有自己的排列顺序。。。
vector<vector<string>> compress(1);
//用于匹配结点及其对应的树节点
map<vector<string>, TreeNode*> m_node;
//用于匹配压缩的结点和对应的权重表
map<TreeNode*, vector<Cnode>*> m_weight;

TreeNode* st = new TreeNode;//用于作为未开始状态的压缩节点。
TreeNode* tn = new TreeNode;//用于存放一层的最终树结果
vector<vector<string>>name(1), name2(1);//name 是最初的状态；name2 是每一轮的压缩结果
vector<vector<string>> nametemp, nametemp2;
vector<Cnode> vcnode, vcnodetemp;

vector <map<string, TreeNode*> > vmnode;

double distance_result = 1000000;
double distance_temp;
double ratio_result = 1;
vector<double> weight_compress;
vector<string> namet;

//to change a double to string
string DoubleToString(double d)
{
	string str;
	stringstream ss;
	ss << d;
	ss >> str;
	return str;
}


//show,传入参数包括压缩的结果vector，压缩后的name列表，压缩率
void show(vector<Cnode> vc, vector<vector<string>> vvsname,double dw){
	ofstream ofile;
	 
	string filename = "d:\\" + DoubleToString(dw) + ".json";

	//ofile.open("d:\\5.json");
	ofile.open(filename,ofstream::app);
	ofile << "{\n\"nodes\":\n[\n";
	for (size_t i = 0; i < vvsname.size(); i++)
	{
		ofile << "{\"id\":";
		for (size_t j = 0; j < vvsname[i].size(); j++)
		{
			if (j == 0 )
			{
				ofile << "\"";
			}
			ofile << vvsname[i][j];
			cout << vvsname[i][j];
			if (j == vvsname[i].size() - 1)
			{
				ofile << "\"";
			}
			

		}
		if (i != vvsname.size()-1)
		{
			ofile << "},\n";
		}
		else
		{
			ofile << "}\n],\n\"links\":[\n";
		}
	}

	for (size_t i = 0; i < vc.size(); i++)
	{
		vector<vector<string>> fn;
		vector<vector<string>> sn;
		double w;
		fn = vc[i].get_fnode();
		sn = vc[i].get_snode();
		w = vc[i].get_weight();
		ofile << "{\"source\":";
		for (size_t j = 0; j < fn.size(); j++)
		{
			for (size_t m = 0; m < fn[j].size(); m++)
			{
				if (m == 0)
				{
					ofile << "\"";
				}
				ofile << fn[j][m];
				if (m == fn[j].size() - 1)
				{
					ofile << "\",";
				}
			}
		}
		ofile << "\"target\":";
		for (size_t j = 0; j < sn.size(); j++)
		{
			for (size_t m = 0; m < sn[j].size(); m++)
			{
				if (m == 0)
				{
					ofile << "\"";
				}
				ofile << sn[j][m];
				if (m == sn[j].size() - 1)
				{
					ofile << "\",";
				}
			}
		}
		ofile << "\"weight\": " << w << "}";
		if (i != vc.size()-1)
		{
			ofile << ",\n";
		}
		else
		{
			ofile << "]\n}";
		}
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
	int ii = 0;//用于name中一个vector只有一个元素
	while (fin >> toread){
		if (toread == "http://biomine.cs.helsinki.fi/bm/links.cgi")
		{
			int flag = 0;
			string temp;
			string temp_s;//用于临时存放node。

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
							TreeNode *tn = new TreeNode;
							m_node[name[ii]] = tn;
							//	vmnode.push_back(m_node); map 本身就是一个list 不需要新的vector来存储- -而我是不是傻。。。。
							tn->setData(name[ii]);
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
	}//注意结束条件的判断
	//看vcnode 以及name
	fin.close();
	vcnodetemp = vcnode;//每完成一次压缩过程重新赋值一次
	m_weight[st] = &vcnode;

	//开始压缩的过程
	double weight_temp = 0;
	double weight_result = 0;
	vector<vector<string>> name_temp(1);//每一次压缩之后的结点组合情况
	vector<vector<string>> name_result(1);//最后保证的压缩结果
	vector<Cnode> vcnode_temp;//每一次压缩之后的图的状态
	vector<Cnode> vcnode_result;//最后保存的压缩图

	vcnode_result = vcnode;
	double oldsize = vcnode.size();
	name.resize(ii);//之前多加了1 所以有一个是空的，此时给它还原。以防之后出现问题。
	name2 = name;
	show(vcnode_result, name2, 1);
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
				//选择了要合并的两个node，并创建新的namevector进行存储。name[i]以及name[j]是我们合并的节点，存入TreeNode中，同时设置父子节点
				TreeNode* tn_temp = new TreeNode;


				/*
				首先计算self-node的weight
				若不为零则创建新的cnode 以及vcnode进行存储。并*/

				//此处有问题- -如何判断是否是完成了一轮遍历之后的赋值
				vcnodetemp = vcnode;
				nametemp2 = name;//每完成一次压缩过程重新赋值一次
				vector<Cnode> ::iterator itertemp = vcnodetemp.begin();


				//将要合并的结点汇总成新的结点，此时应该是
				name_temp[0].insert(name_temp[0].end(), name[i].begin(), name[i].end());
				name_temp[0].insert(name_temp[0].end(), name[j].begin(), name[j].end());

				tn_temp->setData(name_temp[0]);
				tn_temp->setChildl(m_node[name[i]]);
				tn_temp->setChildr(m_node[name[j]]);
				m_node[name[i]]->setParent(tn_temp);
				m_node[name[j]]->setParent(tn_temp);

				while (itertemp != vcnodetemp.end())
				{
					if (isequal(itertemp->get_fnode(), name[i]) == 0 || isequal(itertemp->get_fnode(), name[j]) == 0)//说明存在 要查找的name 由于不分前后所以必须双重判断
					{
						if (isequal(itertemp->get_snode(), name[j]) == 0 || isequal(itertemp->get_snode(), name[i]) == 0)
						{
							weight_temp += itertemp->get_weight();
							//此处需要记录原来的weight不用再次遍历。这样有一个vector就可以直接写一个函数输出结果。因为可能存在无关联边的情况，所以涉及到补零的问题。
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
					//判断哪个结点的子结点数比较多，将零补至相同的个数。
					weight_compress = isenough(weight_compress, cnode_t.get_fnode(), cnode_t.get_snode());
					distance_temp = single_distance(weight_temp, weight_compress);
					if (distance_temp > distance_result)
						goto loop;
					weight_temp = 0;
					weight_compress.clear();
				}

				//此处删除已经合并的结点，方便之后进行遍历
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
				//组合出新的name 即 第一轮分组已经结束
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
				vcnode_temp = add_(vcnodetemp, vcnode_temp);//将剩下的黏贴过来。		
				vcnode_result = vcnode_temp;//作为第一轮结果保存
				distance_result = distance_temp;//作为第一轮结果保存
				name2 = name_temp;//作为第一轮 name的结果保存。
				tn = tn_temp;
				namet = name_temp[0];

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
		//此处需要匹配权重表与压缩节点
		m_weight[tn] = &vcnode_result;
		m_node[namet] = tn;
		compress.push_back(namet);
		show(vcnode_result,name2,ratio_result);
		cout << ratio_result << "\n";
		cout << distance_result;
	}


	show(vcnode_result, name2, ratio_result);
}
