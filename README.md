# C Generic Data Containers

- [Introduction](#introduction)
  - [Why libamxc?](#why-libamxc)

## Introduction

Libamxc is a library providing generic re-usable data containers.

The library provides:

- Double Linked List
- Dynamic Array
- Hash Table
- Ring Buffer
- Dynamic String
- Queues
- Stacks
- Variants

### Why libamxc?

- To fulfill some requirements:
  - Need for linked list and hash table (key value pairs) in which any kind of data can be stored (like pointers to structures, simple types, ... ).
  - The linked list or hash table can contain different kinds of data in one list or hash table. Most implementations available can create linked lists, arrays, hash table, but only server one type.
  - Dynamic strings and arrays, in memory allocation. Can grow and shrink (automatically). Static allocated string or arrays often ok, but once in a while they are too small. Either you over-allocate to be able to handle the rare-cases or make sure that extra memory can be allocated when needed and freed again when not needed anymore.
  - A dynamic (in memory allocation) ring buffer, that support read and write functions.

When using C++ there is a good choice in possible data container implementations, event the standard is supporting data containers.

- [Boost](https://www.boost.org/doc/libs/?view=category_containers)
  - Array, Bitmap, Circular Buffer, Property Map, Property tree, ...
- [Qt](https://doc.qt.io/qt-5/containers.html)
  - QList, QLinkedList, QVector, QMap, QHash, ...
- [C++ STL](https://embeddedartistry.com/blog/2017/8/2/an-overview-of-c-stl-containers)
  - dynamic arrays, queues, stacks, ...

While on C there is no standard library for data containers.

It is even hard to find generic implementations:
- [Search Results](https://www.google.com/search?q=data+containers+C&oq=data+containers+C&aqs=chrome..69i57j69i61j0j69i61l2j0.3913j0j4&sourceid=chrome&ie=UTF-8)

There are existing implementations, all different, all with different focus and different approach
- [bkthomps/Containers](https://github.com/bkthomps/Containers)
- [SGLIB](http://sglib.sourceforge.net/)
- [ccl](https://code.google.com/archive/p/ccl/source)
- and maybe more ...

None of c libraries found, fulfill all requirements and do not always comply with the coding guidelines of the project (see [Coding Guidelines](https://gitlab.com/soft.at.home/ambiorix/ambiorix/blob/master/doc/CODING_GUIDELINES.md)

So either:

- select a project, fork from it, update all code according to the guidelines and add all needed features
- or create a new library and implement everything needed.

The second option was chosen. 

Is this library better then others? Absolutely not. It just better fits the needs.

