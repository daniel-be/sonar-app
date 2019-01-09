package com.example.android.sonar.ble;

@SuppressWarnings("unchecked")
public class RingBuffer<T> {

    private final T[] buffer;          // queue elements
    private int count = 0;          // number of elements on queue
    private int indexOut = 0;       // index of first element of queue
    private int indexIn = 0;       // index of next available slot

    // cast needed since no generic array creation in Java
    public RingBuffer(int capacity) {
        this.buffer = (T[]) new Object[capacity];
    }

    public boolean isEmpty() {
        return this.count == 0;
    }

    public boolean isFull() {
        return this.count == this.buffer.length;
    }

    public int size() {
        return this.count;
    }

    
    public void clear() {
        this.count = 0;
    }

    public void push(T item) {
        if (this.count == this.buffer.length) {
            System.out.println("Ring buffer overflow");
//            throw new RuntimeException("Ring buffer overflow");
        }
        this.buffer[this.indexIn] = item;
        this.indexIn = (this.indexIn + 1) % this.buffer.length;     // wrap-around
        if (this.count++ == this.buffer.length) {
            this.count = this.buffer.length;
        }
    }

    public T pop() {
        if (this.isEmpty()) {
            System.out.println("Ring buffer pop underflow");

//            throw new RuntimeException("Ring buffer underflow");
        }
        T item = this.buffer[this.indexOut];
        this.buffer[this.indexOut] = null;                  // to help with garbage collection
        if (this.count-- == 0) {
            this.count = 0;
        }
        this.indexOut = (this.indexOut + 1) % this.buffer.length; // wrap-around
        return item;
    }

    public T next() {
        if (this.isEmpty()) {
            System.out.println("Ring buffer next underflow");
//            throw new RuntimeException("Ring buffer underflow");
        }
        return this.buffer[this.indexOut];
    }


}