class DSU {    
    private:
        int *p, *rank, s;
    public:
        DSU(int size) { 
            s = size;
            p = new int[size + 1];
            rank = new int[size + 1];
            reset();
        }

        ~DSU() {
            delete[] p;
            delete[] rank;
        }

        void reset() {
            for (int i = 0; i <= s; i++) {
                p[i] = i;
                rank[i] = 1;
            }
        }
        
        int find(int a) {
            if (a == p[a]) return a;
            return p[a] = find(p[a]);
        }
        bool same(int a, int b) {
            return find(a) == find(b);
        }
        void uni(int a, int b) {
            if (same(a, b)) return;
            int rA = rank[find(a)], rB = rank[find(b)];
            if (rA > rB) { p[find(b)] = p[find(a)]; }
            else if (rA == rB) { p[find(b)] = p[find(a)]; rank[p[find(a)]]++; }
            else { p[find(a)] = p[find(b)]; }
        }
};