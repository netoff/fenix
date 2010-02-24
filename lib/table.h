#pragma once

#include "common.h"
#include "model.h"

static const unsigned int max_str_length = 2 * 1024;
static const unsigned int max_text_length = 128 * 1024; 

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace model
			{
				struct DB
				{
					DB(const string& h, int p)
					:host(h),port(p){}
					
					string host;
					int port;
				};
				
				struct Row
				{
					Row(TCMAP* row)
					:_row(row)
					{
					}
					
					Row(const Row& rhs)
					:_row(rhs._row)
					{						
					}
					
					Row& operator=(const Row& rhs)
					{
						if(_row)
						{
							tcmapdel(_row);
						}
						
						_row = tcmapdup(rhs._row);
						
						return *this;
					}
					
					~Row()
					{
						if(_row)
						{
							tcmapdel(_row);
						}
					}
					
					TCMAP* _get_row()
					{
						tcmapdup(_row);
					}
				private:
					TCMAP* _row;
				};
				
				struct RowSet
				{
					RowSet(RDBQRY* query)
					:_list(tcrdbqrysearch(query))
					{
					}
					
					~RowSet()
					{
						if(_list)
						{
							tclistdel(_list);
						}
					}
					
					bool empty()
					{
						return !(tclistnum(_list) > 0);	
					}
					
					vector<string> keys()
					{
						vector<string> ret;
						
						int i, key_size;
						char* key;
						
						for(i = 0; i < tclistnum(_list); i++)
						{
							key = tclistval(_list, i, &key_size);
							
							if(key)
							{
								ret.push_back(string(key, key_size));
							}
						}
						
						return ret;						
					}
					
					vector<Row> rows()
					{
						//not implemented yet
						return vector<Row>();
					}
				private:
					RowSet(const RowSet& rhs){}
					RowSet& operator=(const RowSet& rhs){}
					
					TCLIST* _list;
				};
				
				struct Query
				{					
					Query(DB db)
					:_db(new model::Database(db.host, db.port)),_query(tcrdbqrynew(_db->get_db())){}
					
					~Query()
					{
						if(_query)
						{
							tcrdbqrydel(_query);
						}
					}
					
					Query& add_string(const string& col, const string& val)
					{
						tcrdbqryaddcond(_query, col.c_str(), RDBQCSTREQ, val.c_str());
						
						return *this;
					}
					
					Query& add_int(const string& col, const string& val)
					{
						tcrdbqryaddcond(_query, col.c_str(), RDBQCNUMEQ, val.c_str());
						
						return *this;
					}
					
					Query& add_limit(int limit)
					{
						tcrdbqrysetlimit(_query, limit, 0);
						
						return *this;
					}
					
					//return keys of rows
					vector<string> get_keys() const
					{
						vector<string> ret;
												
						RowSet result_set(_query);
						
						if(!result_set.empty())
						{
							return result_set.keys();
						}
						
						return ret;
					}
				private:
					Query(const Query& rhs) {}
					Query& operator=(const Query& rhs) {}
					
					auto_ptr<model::Database> _db;
					RDBQRY* _query;					
				};				
				
				
				class Base
				{
				public:
					Base(){}
					
					virtual ~Base(){}
					
					virtual bool find(const string& id) = 0;
					virtual bool save(bool fail_on_assoc = false) = 0;
					
					virtual string id_s() = 0;
					virtual int64_t id() = 0;
					
				};
				
				template<class T>
				class Table:public Base
				{
				public:
					typedef T Type;
					
					Table(DB db, const string& type)
					:_id("-1"),_db(db),_cols(tcmapnew()),_type(type) 
					{
					}
					
					virtual ~Table()
					{
						if(_cols)
						{
							tcmapdel(_cols);
						}
					}				
					
					int64_t id()
					{
						int64_t ret = 0;
						
						if(get_param(_id, ret))
						{
							return ret;
						}
						
						return -1;
					}
					
					virtual string id_s()
					{
						if(!this->_id.empty())
						{
							return this->_id;
						}
						
						return "";
					}
					
					virtual bool save(bool fail_on_assoc = false)
					{
						//create new
						if(this->_id.empty() || this->_id == "-1")
						{							
							if(this->_before_create())
							{							
								//create logic
								typedef pair<string, shared_ptr<Base> > assoc;
								
								foreach(assoc p, _assocs)
								{
									if(p.second->save(fail_on_assoc))
									{
										this->_set(p.first + "_id", p.second->id_s());
									}
									else
									{
										if(fail_on_assoc)
										{
											return false;
										}									
									}									
								}
								
								shared_ptr<model::Database> db(new model::Database(_db.host, _db.port));
								
								string guid = _get_uid(db);
								
								if(!guid.empty() && guid != "-1" && _cols)
								{
									//set type
									_set("type", _type);
									
									_on_create(guid);
									
									if(tcrdbtblput(db->get_db(),
											(void*)guid.c_str(), strlen(guid.c_str()), _cols))
									{
										this->_id = guid;
										return true;
									}								
								}
							}
						}
						else
						{
							if(this->_before_save())
							{							
								//save logic
								typedef pair<string, shared_ptr<Base> > assoc;
								
								foreach(assoc p, _assocs)
								{
									if(p.second->save(fail_on_assoc))
									{
										this->_set(p.first + "_id", p.second->id_s());
									}
									else
									{
										if(fail_on_assoc)
										{
											return false;
										}									
									}									
								}
								
								shared_ptr<model::Database> db(new model::Database(_db.host, _db.port));
								
								if(!_id.empty() && _id != "-1")
								{	
									if(tcrdbtblput(db->get_db(),
											(void*)_id.c_str(), strlen(_id.c_str()), _cols))
									{
										return true;
									}								
								}
							}
						}
						
						return false;
					}
					
					virtual bool find(const string& key)
					{	
						shared_ptr<model::Database> db(new model::Database(_db.host, _db.port));			
						
						TCMAP* found = tcrdbtblget(db->get_db(), (void*)key.c_str(), strlen(key.c_str()));
						
						if(found)
						{
							//reset old data
							if(_cols)
							{
								tcmapdel(_cols);
							}
							
							_cols = found;
							_id = key;
							
							return true;
						}
						
						return false;
					}	
					
					virtual bool find(const Query& query)
					{
						vector<string> keys = query.get_keys();
						
						if(!keys.empty())
						{
							//find first
							this->find(keys[0]);
							
							return true;
						}
						
						return false;
					}
					
					vector<shared_ptr<Type> > find_all(const Query& query)
					{
						vector<shared_ptr<Type> > ret;
						
						vector<string> keys = query.get_keys();
						
						foreach(string id, keys)
						{
							shared_ptr<Type> p(new T(_db));
							
							if(p->find(id))
							{
								ret.push_back(p);
							}
						}
						
						return ret;
					}
					
					virtual bool exists(const Query& query)
					{
						return !(query.get_keys().empty());
					}
					
					virtual bool found()
					{
						return (_id != "-1");
					}
				protected:
					virtual bool _before_save() { return true; }
					virtual bool _before_create() { return true; }
					virtual void _on_create(const string& id) {}
					
					void _set(const string& col, const string& val)
					{
						tcmapput2(_cols, col.c_str(), val.c_str());
					}
					
					bool _get(const string& col, string& val) const
					{
						char* v = tcmapget2(_cols, col.c_str());
						
						if(v)
						{
							val.assign(v);
							
							return true;
						}
						
						return false;
					}	
					
					map<string, shared_ptr<Base> > _assocs;
					
					string _id;
					DB _db;
				private:
					string _get_uid(shared_ptr<model::Database> db)
					{
						string ret = "-1";
						
						int64_t id = tcrdbtblgenuid(db->get_db());
						
						if(id > -1)
						{
							try
							{
								ret = lexical_cast<string>(id);
							}
							catch(bad_lexical_cast&)
							{
							}
						}
					
						return ret;
					}
					
					Table(const Table& rhs);
					Table& operator=(const Table& rhs);

					TCMAP* _cols;	
					
					string _type;													
				};
			}
		}
	}
}

#define STR_FIELD(field) 			\
public:						\
string field() 					\
{ 						\
	string val;				\
						\
	if(this->_get(#field, val))		\
	{					\
		return val;			\
	}					\
						\
	return "";				\
}						\
Type& field(const string& val)			\
{						\
	string s(val);				\
	s.resize(max_str_length);		\
						\
	this->_set(#field, val);		\
	return *this;				\
}


#define STR_FIELD_PRIV(field) 			\
private:					\
string field() 					\
{ 						\
	string val;				\
						\
	if(this->_get(#field, val))		\
	{					\
		return val;			\
	}					\
						\
	return "";				\
}						\
Type& field(const string& val)			\
{						\
	string s(val);				\
	s.resize(max_str_length);		\
						\
	this->_set(#field, val);		\
	return *this;				\
}						\
public:

#define STR_FIELD_A(field) 			\
private:					\
	string _##field;			\
public:						\
string field() 					\
{ 						\
	return _##field;			\
}						\
Type& field(const string& val)			\
{						\
	string s(val);				\
	s.resize(max_str_length);		\
						\
	_##field = s;				\
	return *this;				\
}

#define FOREIGN_KEY_FIELD(field)		\
public:						\
template <class T>				\
shared_ptr<T> field()				\
{						\
	shared_ptr<T> p(new T());		\
	p->find(field##_id());			\
						\
	return p;				\
}						\
void field(shared_ptr<Base> ref)		\
{						\
	this->_assocs[#field] = ref;		\
}						\
string field##_id()				\
{						\
	string ret;				\
	if(this->_get(string(#field) + "_id", ret))\
	{					\
		return ret;			\
	}					\
						\
	return "-1";				\	
}

#define HAS(field, type, foreign_key)		\
public:						\
template <class T>				\
vector<shared_ptr<T> > field()			\
{						\
	shared_ptr<T> p(new T(_db));		\
						\
	return p->find_all(Query(_db).		\
		add_string("type", #type).	\
		add_string(#foreign_key, _id));	\
}
	
	
	
	
	
