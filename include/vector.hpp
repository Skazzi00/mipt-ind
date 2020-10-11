#include <cstdlib>
#include <cstddef>
#include <iterator>
#include <new>
#include <algorithm>


namespace mipt {
    const size_t MIN_DATA_SIZE = 8;
    const size_t MAX_PRINT_CNT = 16;

#ifdef MARKER_PROTECTION
    using MarkerType = size_t;
    const size_t MARKER_VALUE = 0xBADF00D;
    const size_t MARKER_SIZE = sizeof(MarkerType);
#else
    const size_t MARKER_SIZE = 0;
#endif

    template<typename T>
    struct Vector {
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        enum class Status {
            OK = 0,
            INVALID = 1,
            CORRUPTED_STRUCT = 2,
            CORRUPTED_DATA = 3,
            NULL_PTR = 4,
            NULL_DATA = 5
        };

    private:
        static const char *statusToStr(Status st) {
            switch (st) {
                case Status::OK:
                    return "OK";
                case Status::INVALID:
                    return "INVALID";
                case Status::CORRUPTED_STRUCT:
                    return "CORRUPTED STRUCT";
                case Status::CORRUPTED_DATA:
                    return "CORRUPTED DATA";
                case Status::NULL_PTR:
                    return "NULL POINTER";
                case Status::NULL_DATA:
                    return "DATA NULL POINTER";
                default:
                    return "UNKNOWN";
            }
        }

    public:

        Vector() noexcept: mSize(0), mCapacity(0), mData(nullptr) {} // O(1)


        void swap(Vector &other) noexcept { // O(1)
            using std::swap;
            swap(mData, other.mData);
            swap(mSize, other.mSize);
            swap(mCapacity, other.mCapacity);
#ifdef MARKER_PROTECTION
            swap(beginMarker, other.beginMarker);
            swap(endMarker, other.endMarker);
#endif
        }

        Vector(Vector &&other) noexcept: Vector() { // O(1)
            swap(other);
        }

        Vector &operator=(Vector &&other) noexcept { // O(1)
            swap(other);
            return *this;
        }

        Vector(const Vector &other) : Vector() { // O(N) strong
            pointer newData = nullptr;
            if (other.mData && !other.empty()) {
                newData = static_cast<pointer>(operator new(
                        other.size() * sizeof(value_type) + 2 * MARKER_SIZE));
#ifdef MARKER_PROTECTION
                setMarkers(newData, other.size());
#endif
            }

            auto data = reinterpret_cast<pointer>(reinterpret_cast<char *>(newData) + MARKER_SIZE);
            size_type i = 0;
            try {
                for (; i < other.size(); i++) {
                    new(data + i) value_type(other[i]);
                }
            }
            catch (...) {
                while (i--) {
                    data[i].~value_type();
                }
                operator delete(newData);
                throw;
            }

            mData = newData;
            mSize = other.size();
            mCapacity = other.size();
        }

        Vector &operator=(Vector const &other) { // O(N) strong
            Vector tmp(other);
            swap(tmp);
            return *this;
        }

        ~Vector() noexcept { // O(N)
            clear();
            operator delete(mData);
        }

        reference operator[](size_type i) noexcept { // O(1)
            return data()[i];
        }

        const_reference operator[](size_type i) const noexcept { // O(1)
            return data()[i];
        }

        pointer data() noexcept { // O(1)
            if (!mData) return mData;
            return reinterpret_cast<pointer>(reinterpret_cast<char *>(mData) + MARKER_SIZE);
        }

        const_pointer data() const noexcept { // O(1)
            if (!mData) return mData;
            return reinterpret_cast<pointer>(reinterpret_cast<char *>(mData) + MARKER_SIZE);
        }

        [[nodiscard]] size_type size() const noexcept { // O(1)
            return mSize;
        }

        [[nodiscard]] size_type capacity() const noexcept { // O(1)
            return mCapacity;
        }

        reference front() noexcept { // O(1)
            return data()[0];
        }

        const_reference front() const noexcept { //O(1)
            return data()[0];
        }

        reference back() noexcept { // O(1)
            return data()[mSize - 1];
        }

        const_reference back() const noexcept { // O(1)
            return data()[mSize - 1];
        }

        void push_back(const_reference val) { // O(1)* strong
            if (size() == capacity()) {
                push_back_realloc(val);
            } else {
                new(data() + size()) value_type(val);
                mSize++;
            }
        }

        void pop_back() noexcept { // O(1)
            data()[--mSize].~value_type();
        }

        [[nodiscard]] bool empty() const noexcept { // O(1)
            return size() == 0;
        }

        void reserve(size_t newCapacity) { // O(N) strong
            if (newCapacity <= capacity()) return;

            Vector<value_type> tmp;
            tmp.mData = static_cast<pointer>(operator new(newCapacity * sizeof(value_type) + 2 * MARKER_SIZE));
#ifdef MARKER_PROTECTION
            setMarkers(tmp.mData, newCapacity);
#endif
            tmp.mCapacity = newCapacity;

            for (size_t i = 0; i < mSize; i++) {
                tmp.push_back(data()[i]);
            }
            swap(tmp);
        }

        void shrink_to_fit() { // O(N) strong
            if (size() == capacity()) return;
            Vector<value_type> tmp(*this);
            swap(tmp);
        }

        void clear() noexcept { // O(N)
            while (size() != 0) {
                pop_back();
            }
        }

        iterator begin() noexcept { // O(1)
            return data();
        }

        iterator end() noexcept { // O(1)
            return data() + size();
        }

        reverse_iterator rbegin() noexcept { // O(1)
            return end();
        }

        reverse_iterator rend() noexcept { // O(1)
            return begin();
        }

        const_iterator begin() const noexcept { // O(1)
            return data();
        }

        const_iterator end() const noexcept { // O(1)
            return data() + size();
        }

        const_reverse_iterator rbegin() const noexcept { // O(1)
            return end();
        }

        const_reverse_iterator rend() const noexcept { // O(1)
            return begin();
        }

        iterator insert(const_iterator pos, const_reference val) { // O(N) weak
            const difference_type posInd = pos - data();
            push_back(val);
            for (auto i = static_cast<difference_type>(size() - 1); i != posInd; i--) {
                std::swap(data()[i], data()[i - 1]);
            }
            return begin() + posInd;
        }

        iterator erase(const_iterator pos) { // O(N) weak
            return erase(pos, pos + 1);
        }

        iterator erase(const_iterator first, const_iterator last) { // O(N) weak
            auto delta = last - first;
            for (auto i = first - data(); i < static_cast<difference_type>(size()) - delta; i++) {
                data()[i] = data()[i + delta];
            }
            while (delta--) {
                pop_back();
            }
            return const_cast<iterator>(first);
        }

        friend Status validate(Vector const *v) {
            if (!v) return Status::NULL_PTR;

            if (v->mCapacity < v->mSize) return Status::INVALID;

#ifdef MARKER_PROTECTION
            if (v->beginMarker != MARKER_VALUE || v->endMarker != MARKER_VALUE) {
                return Status::CORRUPTED_STRUCT;
            }
#endif

            if (!v->mData) {
                return v->mSize == 0 && v->mCapacity == 0 ? Status::OK : Status::NULL_DATA;
            }

#ifdef MARKER_PROTECTION
            if (*reinterpret_cast<const MarkerType *>(v->mData) != MARKER_VALUE ||
                *reinterpret_cast<const MarkerType *>(v->data() + v->mCapacity) != MARKER_VALUE) {
                return Status::CORRUPTED_DATA;
            }
#endif
            return Status::OK;
        }

        template<typename PrintFunc>
        friend void dump(Vector const *v, PrintFunc printFunc, FILE *logfile = stderr, Status status = Status::OK) {
            fprintf(logfile, "Vector<%s> (%s) [0x%zX] {\n", typeid(value_type).name(), statusToStr(status),
                    reinterpret_cast<size_t>(v));
            if (!v) {
                fprintf(
                        logfile,
                        "\tData unavailable\n"
                        "}\n"
                );
                return;
            }

#ifdef MARKER_PROTECTION
            fprintf(
                    logfile,
                    "\tbeginMarker  = %zX\n"
                    "\tendMarker    = %zX\n",
                    v->beginMarker,
                    v->endMarker
            );
#endif

            fprintf(
                    logfile,
                    "\tmSize        = %zu\n"
                    "\tmCapacity    = %zu\n"
                    "\tmData [0x%zx] {\n",
                    v->mSize,
                    v->mCapacity,
                    reinterpret_cast<size_t>(v->mData)
            );
            if (!v->mData) {
                fprintf(
                        logfile,
                        "\t\tData unavailable\n"
                        "\t}\n"
                );
            } else {

#ifdef MARKER_PROTECTION
                fprintf(
                        logfile,
                        "\t\tbeginMarker = %zX\n"
                        "\t\tendMarker   = %zX\n",
                        *reinterpret_cast<const MarkerType *>(v->mData),
                        *reinterpret_cast<const MarkerType *>(v->data() + v->mCapacity)
                );
#endif

                size_type size = v->mCapacity < MAX_PRINT_CNT ? v->mCapacity : MAX_PRINT_CNT;
                for (size_type i = 0; i < size; ++i) {
                    fprintf(logfile, "\t\t[%zu] = ", i);
                    printFunc(logfile, (*v)[i]);
                    fprintf(logfile, "\n");
                }

                if (size < v->mCapacity) {
                    fprintf(logfile, "\t\t...\n");
                }
                fprintf(logfile, "\t}\n");
            }
            fprintf(logfile, "}\n");
        }

    private:
        void push_back_realloc(const_reference val) { // O(1)* strong
            Vector<value_type> tmp;
            tmp.reserve(calc_new_capacity());
            for (size_type i = 0; i < size(); i++) {
                tmp.push_back(data()[i]);
            }
            tmp.push_back(val);
            swap(tmp);
        }

        size_type calc_new_capacity() {
            return capacity() == 0 ? MIN_DATA_SIZE : capacity() * 2;
        }

#ifdef MARKER_PROTECTION

        static void setMarkers(void *ptr, size_type capacity) {
            *reinterpret_cast<MarkerType *>(ptr) = MARKER_VALUE;
            *reinterpret_cast<MarkerType *>(reinterpret_cast<char *>(ptr)
                                            + capacity * sizeof(value_type) + sizeof(MarkerType)) = MARKER_VALUE;
        }

#endif

#ifdef MARKER_PROTECTION
        MarkerType beginMarker = MARKER_VALUE;
#endif
        size_type mSize{};
        size_type mCapacity{};
        pointer mData;
#ifdef MARKER_PROTECTION
        MarkerType endMarker = MARKER_VALUE;
#endif
    };
}