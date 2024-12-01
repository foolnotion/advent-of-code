#include <aoc.hpp>

namespace rg = std::ranges;

namespace {
    using queue_t = std::deque<i64>;
    using graph_t = Eigen::Matrix<u8, -1, -1>;

    template<typename T>
    using ref = std::reference_wrapper<T>;

    auto parse(std::string const& path) {
        auto lines = aoc::util::readlines(path);
        aoc::dense::set<char> nodes;
        std::vector<std::pair<i64, i64>> pairs;
        pairs.reserve(lines.size());
        for (auto const& line : lines) {
            auto [a, b] = scn::scan<char, char>(line, "Step {} must be finished before step {} can begin.")->values();
            pairs.emplace_back(a-'A', b-'A');
            nodes.insert(a);
            nodes.insert(b);
        }
        std::vector<char> letters(nodes.begin(), nodes.end());
        rg::sort(letters);
        graph_t graph = graph_t::Zero(std::ssize(letters), std::ssize(letters));
        for (auto [i, j] : pairs) { graph(i, j) = 1; }
        return std::tuple{std::move(graph), std::move(letters)};
    }

    struct pool {
        Eigen::Ref<graph_t const> graph;
        std::span<i64> done;
        ref<queue_t> queue;
        ref<std::string> steps;

        pool(Eigen::Ref<graph_t const> g, std::span<i64> d, ref<::queue_t> q, ref<std::string> s, i32 n)
            : graph(g), done(d), queue(q), steps(s)
        {
            workers.reserve(n);
            for (auto i = 0; i < n; ++i) {
                workers.push_back(worker{this});
            }
        }

        struct worker {
            pool* p{nullptr};

            auto assign(i64 i, i64 t) -> void {
                task_id = i;
                task_time = t;
            }

            auto work() -> void {
                if (free() || p->task_in_progress(task_id)) {
                    return;
                }

                if (--task_time == 0) {
                    p->complete_task(task_id);
                    task_id = -1;
                }
            }

            [[nodiscard]] auto free() const -> bool { return task_id == -1; }

            i64 task_id{-1};
            i64 task_time{0};
        };

        [[nodiscard]] auto task_in_progress(i64 task_id) const -> bool {
            return done[task_id] > 0;
        }

        auto complete_task(i64 task_id) -> void {
            steps.get() += static_cast<char>(task_id + 'A');
            auto it = queue.get().end();
            for (auto j = 0; j < graph.cols(); ++j) {
                if (graph(task_id, j) == 1 && --done[j] == 0) {
                    queue.get().push_back(j);
                }
            }
            rg::sort(it, queue.get().end());
        }

        [[nodiscard]] auto work_finished() const {
            return rg::all_of(done, [](auto x){ return x == 0; }) && rg::all_of(workers, &worker::free);
        }

        auto work() -> void {
            rg::for_each(workers, &worker::work); ++ticks;
        }

        auto get_worker() {
            auto it = rg::find_if(workers, &worker::free);
            return std::make_pair(it, it != workers.end());
        }

        std::vector<worker> workers;
        i64 ticks{-1};
    };
} // namespace

template<>
auto advent2018::day07() -> result {
    auto const& [graph, letters] = parse("source/2018/07/input.txt");
    auto compute_order = [](::graph_t graph, i32 n_workers) {
        queue_t queue;
        std::vector<i64> done(graph.cols());
        for (auto [i, s] : lz::enumerate(graph.colwise().sum())) {
            if (done[i] = s; s == 0) { queue.push_back(i); }
        }
        std::string steps;
        pool p(graph, done, queue, steps, n_workers);
        constexpr auto c{61};
        while (!p.work_finished()) {
            p.work();
            if (queue.empty()) { continue; }
            rg::sort(queue);

            for(;;) {
                auto [it, ok] = p.get_worker();
                if (!ok) { break; }
                auto i = queue.front();
                it->assign(i, i+c);
                queue.pop_front();
                if (queue.empty()) { break; }
            }
        }
        return std::make_pair(std::move(steps), p.ticks);
    };
    auto const p1 = compute_order(graph, 1).first;
    auto const p2 = compute_order(graph, 5).second;
    return aoc::result(p1, p2);
}