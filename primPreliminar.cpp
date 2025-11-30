vector<int> AlgoritmosGrafo::rutaHastaBase(int base, int recurso) {
    vector<bool> visitado(N, false);
    vector<int> padre(N, -1);
    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;

    // Construimos MST desde la base
    visitado[base] = true;
    for (auto [v, w] : adj[base]) {
        pq.push({w, base, v});
    }

    while (!pq.empty()) {
        auto [peso, u, v] = pq.top(); pq.pop();
        if (visitado[v]) continue;

        visitado[v] = true;
        padre[v] = u;

        for (auto [vecino, w] : adj[v]) {
            if (!visitado[vecino]) pq.push({w, v, vecino});
        }
    }

    // Reconstruimos ruta desde recurso hacia la base
    vector<int> ruta;
    for (int v = recurso; v != -1; v = padre[v]) {
        ruta.push_back(v);
    }
    return ruta; // ruta: recurso -> base
}
