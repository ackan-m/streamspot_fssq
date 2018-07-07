#include "fssq.h"
#include "param.h"
#include <iostream>
#include <math.h>

namespace std{
  void delayedSorting(Counter quasi_heap[][m], Counter heap[], int root, int gid){
    Counter c = quasi_heap[gid][root]; //サブツリーのルートカウンタ
    c.error *= decayed_rate^(heap[gid].t-c.ut);
    c.cnt *= decayed_rate^(heap[gid].t-c.ut) + 1;
    c.ut = heap[gid].t;

    //flagが立ってなかったらsort終わり
    if(c.delay == false){
      return;
    }
    //葉ノードならreturnする
    if(quasi_heap[gid][root*2].item == "NULL"){
      return;
    }

    //子ノードに対してソート
    delayedSorting(quasi_heap, heap, root*2+1, gid);
    delayedSorting(quasi_heap, heap, root*2+2, gid);

    Counter sml;
    Counter left = quasi_heap[gid][root*2+1];
    Counter right = quasi_heap[gid][root*2+2];
    if(left.cnt > right.cnt){
      sml = right;
      quasi_heap[gid][root*2+2]
    }else{
      sml = left;
      quasi_heap[gid][root*2+1]
    }

    if(c.cnt > sml.cnt){
      quasi_heap[gid][root] = sml;
    }

  }

  void ssqAlgorithm(Counter quasi_heap[][m],StreamHeap streamheap[], string outgoing_chunks,int gid){
    //現在時刻を更新
    streaheap[gid].t += 1;

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
        Counter r = quasi_heap[gid][0]; //rはルートノード
        delayedSorting(quasi_heap, streamheap, 0);

       //rootをcで置き換え
       c.item = outgoing_chunks;
       c.error = r.cnt*decayed_rate^(t-r.ut)+1;
       c.cnt = r.cnt*decayed_rate^(t-r.ut)+1;
       c.delay = true;
       c.ut = heap[gid].t;
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
