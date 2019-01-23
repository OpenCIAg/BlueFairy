#ifndef __CIAG__BLUEFAIRY__LIST__
#define __CIAG__BLUEFAIRY__LIST__

namespace ciag {
    namespace bluefairy {

        template<typename E>
        class List {
        public:
            class Node;
            class Iterator;

            List();
            ~List();

            E add(E e);
            Iterator iterator();
            void clear();

            class Node {
            public:
                E element;
                Node * next;
                Node();
                Node(E element);
                Node(E element, Node * next);
            };

            class Iterator {
            public:
                Iterator(Node* node);
                bool hasNext() const;
                E& next();
            protected:
                Node* node;
            };

        protected:
            Node headNode;
        };

        template<typename E>
        List<E>::List() {
            
        }


        template<typename E>
        E List<E>::add(E e){
            Node * node = new Node(e, this->headNode.next);
            this->headNode.next = node;
            return e;
        }

        template<typename E>
        typename List<E>::Iterator List<E>::iterator(){
            return Iterator(&this->headNode);
        }

        template<typename E>
        void List<E>::clear(){
            while(this->headNode.next != NULL){
                Node * node = this->headNode.next;
                this->headNode.next = node->next;
                delete node;
            }
        }

        template<typename E>
        List<E>::~List() {
            this->clear();
        }

        template<typename E>
        List<E>::Node::Node() {
            this->next = NULL;
        }

        template<typename E>
        List<E>::Node::Node(E element) : element(element){
            this->next = NULL;
        }

        template<typename E>
        List<E>::Node::Node(E element, Node * next) : element(element), next(next){
        }

        template<typename E>
        List<E>::Iterator::Iterator(Node * node) : node(node) {

        }

        template<typename E>
        bool List<E>::Iterator::hasNext() const {
            return this->node->next != NULL;
        }

        template<typename E>
        E& List<E>::Iterator::next() {
            this->node = this->node->next;
            return this->node->element;
        }

    }
}

#endif