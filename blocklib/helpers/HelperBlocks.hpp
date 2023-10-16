#ifndef FAIR_HELPERS_HELPER_BLOCKS_HPP
#define FAIR_HELPERS_HELPER_BLOCKS_HPP

#include <gnuradio-4.0/Block.hpp>

/**
 * TRANSITIONAL: some simple helpers blocks for tests, should go upstream or
 * replaced by upstream blocks.
 */
namespace fair::helpers {

template<typename T>
struct VectorSource : public gr::Block<VectorSource<T>> {
    gr::PortOut<T> out;

    std::vector<T> data;
    std::size_t    _produced = 0;

    void
    settingsChanged(const gr::property_map & /*old_settings*/, const gr::property_map & /*new_settings*/) {
        _produced = 0;
    }

    gr::work::Status
    processBulk(gr::PublishableSpan auto &output) noexcept {
        using enum gr::work::Status;
        const auto n     = std::min(output.size(), data.size() - _produced);
        const auto chunk = std::span(data).subspan(_produced, n);
        std::copy(chunk.begin(), chunk.end(), output.begin());
        output.publish(n);
        _produced += n;
        return _produced == data.size() ? DONE : OK;
    }
};

template<typename T>
struct VectorSink : public gr::Block<VectorSink<T>> {
    gr::PortIn<T>  in;
    std::vector<T> data;

    gr::work::Status
    processBulk(std::span<const T> input) {
        data.insert(data.end(), input.begin(), input.end());
        return gr::work::Status::OK;
    }
};

template<typename T>
struct TagDebug : public gr::Block<TagDebug<T>> {
    gr::PortIn<T>        in;
    gr::PortOut<T>       out;
    std::vector<gr::Tag> seen_tags;
    std::size_t          samples_seen = 0;

    gr::work::Status
    processBulk(std::span<const T> input, std::span<T> output) noexcept {
        std::copy(input.begin(), input.end(), output.begin());
        if (this->input_tags_present()) {
            auto tag = this->input_tags()[0];
            tag.index += static_cast<int64_t>(samples_seen);
            seen_tags.push_back(std::move(tag));
        }
        samples_seen += input.size();
        return gr::work::Status::OK;
    }
};

template<typename T>
struct CountSink : public gr::Block<CountSink<T>> {
    gr::PortIn<T> in;
    std::size_t   samples_seen = 0;

    gr::work::Status
    processBulk(std::span<const T> input) noexcept {
        samples_seen += input.size();
        return gr::work::Status::OK;
    }
};

} // namespace fair::helpers

ENABLE_REFLECTION_FOR_TEMPLATE(fair::helpers::VectorSource, out, data);
ENABLE_REFLECTION_FOR_TEMPLATE(fair::helpers::VectorSink, in, data);
ENABLE_REFLECTION_FOR_TEMPLATE(fair::helpers::TagDebug, in, out);
ENABLE_REFLECTION_FOR_TEMPLATE(fair::helpers::CountSink, in);

#endif