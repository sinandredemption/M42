#include <chrono>
#include <thread>
#include <valarray>

#include <Perft.hpp>

using namespace std::chrono;

int main(int argc, char *argv[]) {
  const fen::FEN f = (argc >= 2) ? fen::load_from_string(argv[1]) : fen::search_explosion_pos;
  // fen::starting_pos
  //const std::valarray<uint64_t> shannon_number = { 20, 400, 8902, 197281, 4865609, 119060324, 3195901860, 84998978956, 2439530234167 };
  // fen::search_explosion_pos
  const std::valarray<uint64_t> shannon_number = { 99, 6271, 568299, 34807627, 3093043991 };
  printf("perft benchmarks\n");
  printf("position: %s\n", fen::export_as_string(f).c_str());
  {
    Perft p(f);
    decltype(auto) store_scope = p.get_zobrist_perft_scope();
    for(size_t depth = 1; depth <= shannon_number.size(); ++depth) {
      store_scope.reset();
      auto start = system_clock::now();
      size_t nds = p.perft(depth);
      auto stop = system_clock::now();
      long dur = duration_cast<nanoseconds>(stop-start).count();
      double sec = 1e-9*dur;
      double kndssec = (double(nds)/sec)*1e-3;
      double kndssec_raw = (double(p.nodes_searched)/sec)*1e-3;
      double hit_rate = double(p.zb_hit) / double(1e-9 + p.zb_hit + p.zb_miss);
      printf("depth=%lu, time=%.3f\t%.3f kN/sec\traw=%.3f kN/sec\tnodes=%lu\tshannon=%lu\thit_rate=%.3f\n",
              depth, sec, kndssec, kndssec_raw, nds, shannon_number[depth-1], hit_rate);
      printf("press enter\n");
      fflush(stdout);
      fgetc(stdin);
    }
  }
  printf("\n");
}
