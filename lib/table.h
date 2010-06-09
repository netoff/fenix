#pragma once

#include "common.h"
#include "model.h"
#include "model_obj.h"

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
				struct Query
				{					
					Query(){}
					
					~Query(){}
					
					Query& add_cond(const string& key, const string& val)
					{
						this->_cols[key] = val;
						
						return *this;
					}
					
					/*
					Query& add_bool;
					Query& add_num;
					*/
					
					string to_json()
					{
						ostringstream out;
						
						out << "{";
						
						typedef pair<string, string> tuple;
						
						bool first = true;
						
						foreach(tuple p, this->_cols)
						{
							string a = p.first;
							string b = p.second;
							
							if(!first)
							{
								out <<", "; 
							}
							
							ostringstream val;
							
							if(a == "_id")
							{
								val << "ObjectId(\"" << b.substr(0, 24) << "\")";
							}
							else
							{
								val << "\"" << b << "\"";
							}
							
							out << " " << a << ": " << val.str();
							
							first = false;
						}
						
						out << "}";
						
						return out.str();
					}
					
				private:
					Query(const Query& rhs);
					Query& operator=(const Query& rhs);
					
					hash _cols;
				};				
				
				
				class ObjBase
				{
				public:
					ObjBase(Database::obj db)
					:_db(db),_id(""),_queried(false){}
					
					virtual ~ObjBase(){}
					
					virtual bool find(const string& id)=0;
					
					virtual bool find(const Query& query)=0;			
					
					virtual bool save(bool fail_on_assoc = false)=0;
					
					virtual string id()
					{
						return this-> _id;
					}
					
					bool new_record()
					{
						return (this->_id == "" && !this->_queried);
					}
					bool found()
					{
						return (this->_id != "" && this->_queried);
					}
					
				protected:
					virtual void unload(DatabaseObj obj) = 0;
					virtual void load(const DatabaseObj obj) = 0; 
					
					Database::obj _db;
					
					string _id;
					bool _queried;					
										
				};
				
				template<class T>
				class Table: public ObjBase
				{
				public:
					typedef T Type;
					typedef shared_ptr<T> obj;
					typedef vector<shared_ptr<T> > collection;
					
					virtual ~Table(){}
					
					static obj get(Database::obj db)
					{
						obj ret(new T(db));						
						return ret;
					}

					virtual bool find(const string& id)
					{	
						if(!id.empty())
						{
							DatabaseObj res = this->_db->find(Query().add_cond("_id", id).to_json(), this->_type); 
							
							this->load(res);
							
							if(!res->is_empty())
							{
								return true;
							}
						}
						
						return false;
					}
					
					virtual bool find(const Query& query)
					{						
						DatabaseObj res = this->_db->find(query.to_json(), this->_type); 
							
						this->load(res);
						
						if(!res->is_empty())
						{
							return true;
						}
						
						return false;
					}					
					
					collection find_all(const Query& query)
					{
						//log::log() << "__find_all: " << query;
						
						collection ret;
						
						DatabaseColl cursor = this->_db->find_all(query.to_json(), this->_type);
						
						foreach(DatabaseObj raw_obj, cursor)
						{
							obj o(Type::get(this->_db));
							
							o->load(raw_obj);
							ret.push_back(o);
						}
						
						return ret;						
					}
					
					collection find_all()
					{
						return this->find_all(Query());
					}
					
					virtual bool save(bool fail_on_assoc = false)
					{
						//create new
						if(this->new_record())
						{							
							if(!this->_before_create())
							{							
								return false;								
							}
						}
						else
						{
							if(!this->_before_save())
							{							
								return false;
							}
						}
						
						typedef pair<string, shared_ptr<ObjBase> > assoc;
								
						foreach(assoc p, _assocs)
						{
							if(p.second->save(fail_on_assoc))
							{
								this->_set(p.first + "_id", p.second->id());
							}
							else
							{
								if(fail_on_assoc)
								{
									return false;
								}									
							}									
						}
						
						if(this->_save(fail_on_assoc))
						{
							this->_on_create(this->_id);
							
							return true;
						}
						
						return false;
					}
					
					
					virtual bool exists(const Query& query)
					{
						return this->_db->exists(query.to_json(), this->_type);
					}
				protected:
					Table(Database::obj db, const string& type)
					:_type(type),ObjBase(db) 
					{
					}
					
					virtual bool _before_save() { return true; }
					virtual bool _before_create() { return true; }
					virtual void _on_create(const string& id) {}
					
					virtual void unload(DatabaseObj obj)
					{
						if(!this->new_record())
						{
							obj->_id = this->_id;							
						}
						
						obj->_cols = this->_cols;
					}
					
					virtual void load(const DatabaseObj obj)
					{
						this->_queried = true;
						
						if(!obj->is_empty())
						{
							this->_id = obj->_id;
							this->_cols = obj->_cols;
						}
						else
						{
							this->_id = "";
							this->_cols.clear();
						}
					}
					
					void _set(const string& col, const string& val)
					{
						this->_cols[col] = val;
					}
					
					bool _get(const string& col, string& val) const
					{
						hash::const_iterator it = this->_cols.find(col);
						
						if(it != this->_cols.end())
						{
							val = it->second;
							return true;
						}
						
						return false;
					}	
					
					map<string, shared_ptr<ObjBase> > _assocs;
				private:
					bool _save(bool fail_on_assoc = false)
					{						
						DatabaseObj raw_obj(new _DatabaseObj());
						
						this->unload(raw_obj);
						
						string id = this->_db->save(this->_id, raw_obj->to_json(), this->_type);
						
						if(!id.empty())
						{
							if(this->new_record())
							{
								this->_id = id;
							}
							
							return true;
						}
							
						return false;
					}
					
					Table(const Table& rhs);
					Table& operator=(const Table& rhs);
					
					string _type;		
					
					hash _cols;
				};
				
				#define TABLE(name) \
				name(Database::obj db)\
				:Table<name>(db, #name){}
			}
		}
	}
}

#define STRING_FIELD(field)	 		\
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


#define STRING_PRIV(field) 			\
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

#define STRING_FIELD_A(field) 			\
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
typename T::obj field()				\
{						\
	typename T::obj p(new T(this->_db));	\
	p->find(field##_id());			\
						\
	return p;				\
}						\
void field(shared_ptr<ObjBase> ref)		\
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
	return "";				\
}

#define HAS(field, type, foreign_key)		\
public:						\
template <class T>				\
typename T::collection field()			\
{						\
	typename T::obj p(new T(this->_db));	\
						\
	return p->find_all(Query().		\
		add_cond(#foreign_key, _id));	\
}
	
	
	
	
	
