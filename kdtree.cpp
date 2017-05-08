#include <cmath>
/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if(first[curDim]!=second[curDim]){
       return first[curDim]<second[curDim];
     }
    return first<second;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
     int dp = 0;
     int dc = 0;
     for(int i = 0; i < Dim; i++){
       dp += pow(potential[i]-target[i],2);
       dc += pow(currentBest[i]-target[i],2);
     }
     if(dp!=dc){
       return dp<dc;
     }
     else{
       return potential<currentBest;
     }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     if(newPoints.empty()){
       points = newPoints;
     }
     else{
       //cout<<"hi"<<endl;
       points = newPoints;
       buildKDTree(points, 0, points.size()-1,0);
     }
}

template <int Dim>
int KDTree<Dim>::findMin(vector<Point<Dim>> a, int start, int end, int curDim)const{

  if(start>=end){
    return start;
  }
  //cout<<"finding"<<endl;
  int min = start;
  for(int i = start; i <= end; i++){
    if(smallerDimVal(a[i],a[min],curDim)){
      min = i;
    }
  }
  return min;
}

template <int Dim>
void KDTree<Dim>::quicksort(vector<Point<Dim>>& a,int start, int end, int curDim){
  //cout<<"sorting"<<endl;
  if(start>=end){
    return;
  }
  for(int i = start; i <= end; i++){
    int min = findMin(a,i,end,curDim);
    if(min!=i){
      Point<Dim> temp = a[i];
      a[i] = a[min];
      a[min] = temp;
    }
  }
}

/*template <int Dim>
void KDTree<Dim>::quicksort(vector<Point<Dim>>& a,int start, int end, int curDim){
  if(start<end){
    int p = partition(a,start,end, curDim);
    quicksort(a,start,p-1,curDim);
    quicksort(a,p+1,end, curDim);
  }
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& a, int start, int end, int curDim){
  Point<Dim> pivot = a[start];
  int i = start-1;
  int j = end+1;
  while(true){
    do{
      i++;
    } while(smallerDimVal(a[i],pivot,curDim));
    do{
      j--;
    } while(smallerDimVal(pivot,a[j],curDim));
    if(i>=j){
      return j;
    }
    std::swap(a[i],a[j]);
  }
}*/


template <int Dim>
void KDTree<Dim>::buildKDTree(vector<Point<Dim>>& newPoints,int start, int end,int curDim){
  //if(newPoints.size()==1) return newPoints
  if(start>=end){
    return;
  }
  //cout<<"building"<<endl;
  //quicksort newPoints
  quicksort(newPoints,start,end,curDim);
  //subvector newPoints[0,(size()-1)/2-1] = buildKDTree(newPoints[0,mid],curDim+1%k)
  buildKDTree(newPoints,start,(int)((start+end)/2)-1,(curDim+1)%Dim);
  //subvector newPoints[(size()-1)/2+1,size] = buildKDTree(newPoints[mid+1,size()],curDim+1%k)
  buildKDTree(newPoints,(int)((start+end)/2)+1, end, (curDim+1)%Dim);
  //return newPoints
}


template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return findNearestNeighbor(query,0,points.size()-1,0);
}

template <int Dim>
double KDTree<Dim>::distance(Point<Dim> a, Point<Dim> b)const{
  double ret = 0.0;
  for(int i = 0; i < Dim; i++){
    ret += pow(a[i]-b[i],2);
  }
  return sqrt(ret);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query, int start, int end, int curDim) const
{
  Point<Dim> ret;
  Point<Dim> curBest;
  Point<Dim> other;
  bool left = false;
  int mid = (start+end)/2;
  int curR = 0;
  //traverse down to the closest point(binary search) ret
  //cout<<"hi"<<endl;
  if(start==end){
    return points[start];
  }
  else{
    //mid = (int)((start+end)/2);
    if(smallerDimVal(query,points[mid],curDim)){
      //cout<<"left"<<endl;
      if(mid-start<1){
        return points[start];
      }
      else{
        curBest = findNearestNeighbor(query,start,mid-1,(curDim+1)%Dim);
      }
      left = true;
    }
    else{
      //cout<<"right"<<endl;
      if(end-mid<1){
        return points[end];
      }
      else{
        curBest = findNearestNeighbor(query,mid+1,end, (curDim+1)%Dim);
      }
      left = false;
    }

    //cout<<"1"<<endl;
    //if the distance from the point to query < curradius replace point
    /*if(distance(points[mid],query)<distance(curBest,query)){
      ret = points[mid];
      curR = distance(points[mid],query);
    }
    //if the distance from the point to query = curradius ret = ret<the point?ret:the point
    else if(distance(points[mid],query)==distance(curBest,query)){
      if(points[mid]<curBest){
        ret = points[mid];
        curR = distance(points[mid],query);
      }
      else{
        ret = curBest;
        curR = distance(curBest,query);
      }
    }
    else{
      ret = curBest;
      curR = distance(curBest,query);
    }*/
    if(shouldReplace(query,curBest,points[mid])){
      ret = points[mid];
      curR = distance(points[mid],query);
    }
    else{
      ret = curBest;
      curR = distance(curBest,query);
    }
    //cout<<"2"<<endl;
    //if the distance > query[curDim] another = find in another subtree
    if(pow(curR,2)>=pow(query[curDim]-(points[mid])[curDim],2)){
      if(left){
        if(end-mid<1){
          other = points[mid];
        }
        else{
          other = findNearestNeighbor(query,mid+1,end, (curDim+1)%Dim);
        }
      }
      else{
        if(mid-start<1){
          other = points[mid];
        }
        else{
          other = findNearestNeighbor(query,start,mid-1,(curDim+1)%Dim);
        }
      }
      //if distance of another < the disctance of ret ret = another
      if(shouldReplace(query,ret,other)){
        return other;
      }
      else{
        return ret;
      }
      //cout<<"3"<<endl;
    }
    return ret;
  }
}
