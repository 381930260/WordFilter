#include <cstdint>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

//关键字过滤算法 358860528@qq.com
//trie树算法
class WordFilter
{
public:
	WordFilter() {}

	~WordFilter()
	{
		Clean(&m_Filter);
	}

	bool Load(const char* filename)
	{
		std::ifstream fin(filename);
		if (!fin.is_open())
		{
			return false;
		}

		std::string inbuf;
		while (!fin.eof())
		{
			// 读取一行
			getline(fin, inbuf, '\n');


			if (inbuf.size() == 0)
			{
				continue;
			}
			AddWord(inbuf.c_str());

		}

		return true;
	}

	void AddWord(const char* word)
	{
		int len = strlen(word);
		Filter* filter = &m_Filter;

		for (int i = 0; i < len; i++)
		{
			unsigned char c = word[i];
			if (i == len - 1)
			{
				filter->m_NodeArray[c].m_Flag |= FilterNode::NODE_IS_END;
				break;
			}
			else
			{
				filter->m_NodeArray[c].m_Flag |= FilterNode::NODE_HAS_NEXT;
			}

			if (filter->m_NodeArray[c].m_NextFilter == NULL)
			{
				Filter* tmpFilter = new Filter();
				filter->m_NodeArray[c].m_NextFilter = tmpFilter;
			}

			filter = (Filter *)filter->m_NodeArray[c].m_NextFilter;
		}
	}

	void AddWords(const std::set<std::string>& wordList)
	{
		for (std::set<std::string>::const_iterator it = wordList.begin();
			it != wordList.end(); it++)
		{
			AddWord(it->c_str());
		}
	}

	void AddWords(const std::vector<std::string>& wordList)
	{
		for (auto it = wordList.begin();it != wordList.end(); it++)
		{
			AddWord(it->c_str());
		}
	}


	int Check(const char* str)
	{
		Filter* filter = NULL;
		for (int i = 0; i < (int)strlen(str) - 1; i++)
		{
			filter = &m_Filter;
			for (int j = i; j < strlen(str); j++)
			{
				unsigned char c = str[j];
				//if ((c >= 'A' && c <= 'Z'))
				//{
				//	c += 32;
				//}

				if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
				{
					break;
				}
				else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
				{
					return i;
				}
				else // NODE_HAS_NEXT
				{
					filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
				}
			}
		}
		return -1;
	}

	void CheckAndModify(char* str, const char replace = '*')
	{
		Filter* filter = NULL;
		for (int32_t i = 0; i < (int)strlen(str) - 1; i++)
		{
			filter = &m_Filter;
			for (uint32_t j = i; j < strlen(str); j++)
			{
				unsigned char c = str[j];
				//if ((c >= 'A' && c <= 'Z'))
				//{
				//	c += 32;
				//}

				if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
				{
					break;
				}
				else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
				{
					for (uint32_t k = i; k <= j; k++)
					{
						str[k] = replace;
					}

					if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_HAS_NEXT)
					{
						filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
					}
					else
					{
						continue;
					}
				}
				else // NODE_HAS_NEXT
				{
					filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
				}
			}
		}
	}

	void CheckAndModify(std::string& str, const char replace = '*')
	{
		Filter* filter = NULL;
		for (int32_t i = 0; i < (int)str.size() - 1; i++)
		{
			filter = &m_Filter;
			for (uint32_t j = i; j < str.size(); j++)
			{
				unsigned char c = str[j];
				//if ((c >= 'A' && c <= 'Z'))
				//{
				//	c += 32;
				//}
				if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
				{
					break;
				}
				else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
				{
					for (uint32_t k = i; k <= j; k++)
					{
						str[k] = replace;
					}

					if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_HAS_NEXT)
					{
						filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
					}
					else
					{
						continue;
					}
				}
				else // NODE_HAS_NEXT
				{
					filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
				}
			}
		}
	}

private:

	struct FilterNode
	{
		char m_Flag;
		void* m_NextFilter;

		enum Flag
		{
			NODE_IS_NULL = 0x00,
			NODE_HAS_NEXT = 0x01,
			NODE_IS_END = 0x10,
		};
		FilterNode() : m_Flag(NODE_IS_NULL), m_NextFilter(NULL) {}
	};

	struct Filter
	{
		FilterNode m_NodeArray[256];
	} m_Filter;

	void Clean(Filter* filter)
	{
		for (uint32_t i = 0; i < 256; i++)
		{
			if (filter->m_NodeArray[i].m_NextFilter)
			{
				Clean((Filter *)filter->m_NodeArray[i].m_NextFilter);
				delete (Filter*)filter->m_NodeArray[i].m_NextFilter;
			}
		}
	}
};


int  main()
{

	WordFilter world;


	world.Load("dirtyword.key");

	//

	//world.AddWord("abc");
	//world.AddWord("你妹");

	std::string ssss = "aaagongchandang nihao a 陆肆事件  你妹";
	
	std::cout << ssss << world.Check(ssss.c_str()) << std::endl;
	world.CheckAndModify(ssss);
	std::cout <<"修改后数据"<< ssss << std::endl;


	std::ifstream fin("dirtyword.key");
	if (!fin.is_open())
	{
		return -1;
	}

	std::string inbuf;
	while (!fin.eof())
	{
		// 读取一行
		getline(fin, inbuf, '\n');


		if (inbuf.size() == 0)
		{
			continue;
		}

		inbuf+= char(rand() % 36 + 97);
		world.CheckAndModify(inbuf);

		std::cout << inbuf << std::endl;

	}

	//system("pause");
	return 0;
}