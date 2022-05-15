# CMSC-123-Implement-Hash-Table

This machine problem solution presents an implementation of the data structure **hash table**.

## About the Machine Problem

The machine problem solved here is how to implement a hash table that is able to handle hashing collisions.

The solution is a `Student` class which to be stored in a hash table. The `Student.familyName` property is used as primary key. A program-generated secondary hash key (`DataID`) is also utilized to distinguish distinct data with the same keys. Chaining is used to handle collisions. A `mod 2` function is used to map hash keys to their respective containers. This is used because the architecture of a computer makes power-of-2 modulo operations very cheap in comparison to modulo operations with other bases.

The Hash Table is implemented as an array (chain) of singly-linked list.

## About CMSC 123

CMSC 123, or better known as __Data Structures__, is a theory and programming course offered for Computer Science students in the [University of the Philippines Cebu](upcebu.edu.ph) during their sophomore year. This course primarily deals with  data structures (DS) and their underlying implementations and is encompassed by the following topics:

- Algorithmic Analysis
- Containers
- Memory Allocations
- List (Arrays)
- Linked List
- List/Linked-list -based DS (e.g. stack, queue, trees, graphs)
- Hash Tables
- Sorting
