// $Id: listmap.tcc,v 1.14 2015-07-26 10:04:17-07 - - $

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::listmap() {
//does nothing, all of the variables are initialized in header
}

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
   while(!empty()){
      erase(begin());
   }
}

//
// listmap::listmap(const listmap& copy)
// 
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::listmap(const listmap& copy) {
   less=*copy->less;
   for(auto itor=copy.begin();itor!=copy.end();++itor){
      insert(*itor);
   }
}

//
// listmap::operator=(const listmap& copy)
//
//template <typename Key, typename Value, class Less>
//listmap<Key,Value,Less> listmap<Key,Value,Less>&::operator=(const listmap& copy) {
//}

//
// listmap::listmap(listmap&& move)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::listmap(listmap&& move) {
   Less less=move.less;
   for(auto itor= move.begin();itor!=move.end();++itor){
      insert(*itor);
   }
   delete move.less;
   while(!move.empty()){
      move.erase(move.begin());
   }
}

//
// listmap::operator(listmap&& move)
//
//template <typename Key, typename Value, class Less>
//listmap<Key,Value,Less>&
//listmap<Key,Value,Less>::
//listmap<Key,Value,Less>& operator=(listmap<Key,Value,Less>&& move) {
//}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   node* newNode=new node (nullptr,nullptr,pair);
   iterator search = begin();
   for(;search!=end();++search){
      if(!less(search->first,pair.first)) break;
   }
   if(search!=end()){
      if(!less(pair.first,search->first)
         &&!less(search->first,pair.first)) 
         search=erase(search);
   }
   newNode->next=search.where;
   newNode->prev=search.where->prev;
   search.where->prev->next=newNode;
   search.where->prev=newNode;
   //TRACE ('l', &pair << "->" << pair);
   return iterator(newNode);
}

//
// iteratorlistmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that){
   TRACE ('l', that);
   for(auto itor=iterator(anchor()->next);itor!=end();++itor){
      if(itor->first==that)
         return itor;
   }
   return end();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   position.where->next->prev=position.where->prev;
   position.where->prev->next=position.where->next;
   iterator to_remove=position;
   ++position;
   delete to_remove.where;
   return position;
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
//cout<<where<<endl;
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

