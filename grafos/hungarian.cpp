/* Dado un grafo bipartito completo (n, m) con n <= m, devuelve
 * el matching máximo de mayor peso. Para obtener el de menor peso,
 * poner los costes de las aristas negativos y negar H.hungarian(). */
#define tipo double
const tipo EPS = 1e-9;
const tipo INF = 1e14;
const int MAXH = 505;
struct Hungarian {
    tipo cost[MAXH][MAXH], lx[MAXH], ly[MAXH], slack[MAXH];
    int n, m, max_match, xy[MAXH], yx[MAXH], slackx[MAXH], prev2[MAXH];
    bool S[MAXH], T[MAXH];
    void init(int _n, int _m){ n = max(_n, _m); m = min(_n, _m);
        forn(i, MAXH) forn(j, MAXH) cost[i][j] = INF; }
    void add_to_tree(int x, int prevx){
        S[x] = true, prev2[x] = prevx; 
        forn(y, n) if(lx[x] + ly[y] - cost[x][y] < slack[y] - EPS)
            slack[y] = lx[x] + ly[y] - cost[x][y], slackx[y] = x;
    }
    void update_labels(){
        tipo delta = INF; 
        forn(y, n) if(!T[y]) delta = min(delta, slack[y]);
        forn(x, n) if(S[x]) lx[x] -= delta;
        forn(y, n) if(T[y]) ly[y] += delta; else slack[y] -= delta;
    }
    void init_labels(){
        memset(lx, 0, sizeof lx); memset(ly, 0, sizeof ly);
        forn(x, n) forn(y, n) lx[x] = max(lx[x], cost[x][y]);
    }
    void augment() {
        if(max_match == n) return; 
        int x, y, root, q[MAXH], wr = 0, rd = 0; 
        memset(S, false, sizeof(S)), memset(T, false, sizeof(T)); 
        memset(prev2, -1, sizeof(prev2)); 
        forn(x, n) if(xy[x] == -1){
            q[wr++] = root = x, prev2[x] = -2;
            S[x] = true; break; }
        forn(y, n) slack[y] = lx[root] + ly[y] - cost[root][y], slackx[y] = root;
        while(true){
            while(rd < wr){
                x = q[rd++];
                for(y = 0; y < n; y++) if(cost[x][y] == lx[x] + ly[y] && !T[y]){
                    if(yx[y] == -1) break; T[y] = true; 
                    q[wr++] = yx[y], add_to_tree(yx[y], x); }
                if(y < n) break; }
            if(y < n) break; 
            update_labels(), wr = rd = 0; 
            for(y = 0; y < n; y++) if(!T[y] && slack[y] == 0){
                if(yx[y] == -1){x = slackx[y]; break;}
                else {
                    T[y] = true; 
                    if (!S[yx[y]]) q[wr++] = yx[y], add_to_tree(yx[y], slackx[y]);
                }}
            if(y < n) break; }
        if(y < n){
            max_match++; 
            for(int cx = x, cy = y, ty; cx != -2; cx = prev2[cx], cy = ty)
                ty = xy[cx], yx[cy] = cx, xy[cx] = cy;
            augment(); }
    }
    tipo hungarian(){
        tipo ret = 0; max_match = 0, memset(xy, -1, sizeof(xy)); 
        memset(yx, -1, sizeof(yx)), init_labels(), augment(); //steps 1-3
        forn(x, m) ret += cost[x][xy[x]]; 
        return ret;
    }
};
