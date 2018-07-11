#include "fssq.h"
#include "param.h"
#include <iostream>
#include <math.h>
//seg2だ----
namespace std{
  void delayedSorting(Counter quasi_heap[][m], StreamHeap streamheap[], int root, int gid){
    // if(root >= m){
    //   return;
    // }
    //葉ノードならreturnする
    if(2*root + 1 >= m){
      return;
    }
    if(quasi_heap[gid][root*2+1].item == "NULL"){
      return;
    }

    Counter c = quasi_heap[gid][root]; //サブツリーのルートカウンタ
    c.error *= pow(DECAYED_RATE, streamheap[gid].t - c.ut);
    c.cnt *= pow(DECAYED_RATE, streamheap[gid].t - c.ut);
    c.ut = streamheap[gid].t;

    //flagが立ってなかったらsort終わり
    if(c.delay == false){
      return;
    }

    //子ノードに対してソート
    delayedSorting(quasi_heap, streamheap, root*2+1, gid);
    // if(root*2+2 >= m){
    //   return;//サイズがmを超えた時セグメン出る??から回避する
    // }
    delayedSorting(quasi_heap, streamheap, root*2+2, gid);

    Counter sml; //子ノードの小さいほう
    Counter left = quasi_heap[gid][root*2+1];
    Counter right = quasi_heap[gid][root*2+2];

    //親と子を比較し交換する
    if(left.cnt > right.cnt){
      sml = right;
      if(sml.cnt < c.cnt){
        quasi_heap[gid][root*2+2] = c;
        quasi_heap[gid][root*2+2].delay = true;
        quasi_heap[gid][root] = sml;
      }
    }else{
      sml = left;
      if(sml.cnt < c.cnt){
        quasi_heap[gid][root*2+1] = c;
        quasi_heap[gid][root*2+1].delay = true;
        quasi_heap[gid][root] = sml;
      }
    }

    quasi_heap[gid][root].delay = false;

  }

  void ssqAlgorithm(Counter quasi_heap[][m],StreamHeap streamheap[], string outgoing_chunks,int gid){
    //現在時刻を更新
    streamheap[gid].t += 1;
    //outgoing_chunkがヒープ内にあるか探す
    int length;
    for(length = 0; length < m; length++){
      if(quasi_heap[gid][length].item == outgoing_chunks){
        cout << "attazo---" << endl;
        break;
      }
    }

    Counter c; //新しいカウンタを作る
    if(length < m){//ヒープにある
      c = quasi_heap[gid][length];
      c.error *= pow(DECAYED_RATE, streamheap[gid].t - c.ut);
      c.cnt = c.cnt * pow(DECAYED_RATE, streamheap[gid].t - c.ut) + 1;
      c.delay = true;
      c.ut = streamheap[gid].t;
      //heap内のカウンタ更新
      quasi_heap[gid][length] = c;
    }else{//ヒープにない
      if(streamheap[gid].size == m){//heapがfullなら
        //rootからソート
        delayedSorting(quasi_heap, streamheap, 0, gid);
        cout <<"length  " << length << endl;
        Counter r = quasi_heap[gid][0]; //rはソート後のroot
       //rootをcで置き換え
       c.item = outgoing_chunks;
       c.error = r.cnt+1;
       c.cnt = r.cnt+1;
       c.delay = true;
       c.ut = streamheap[gid].t;
       quasi_heap[gid][0] = c;

     }else{//heapがfullでないなら
       //heapのサイズを増やす
       streamheap[gid].size += 1;

       c.item = outgoing_chunks;
       c.error = 0;
       c.cnt = 1;
       c.delay = false;
       c.ut = streamheap[gid].t;

       //heapに挿入
       quasi_heap[gid][streamheap[gid].size-1] = c;
       cout << "満タンじゃないよ" << endl;
     }
   }
 }
}
