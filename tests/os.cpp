#include <new.h>
#include <cstdlib> // link to the os for now this is the only mixing used for now
namespace os
{
    void *operator_new(std::size_t size)
    {
        return malloc(size);
    }

    void *operator_new_array(std::size_t size)
    {
        return malloc(size);
    }
    void *operator_new_aligned(std::size_t size, std::size_t alignment)
    {
        return aligned_alloc(alignment, size);
    }
    void operator_delete(void *ptr)
    {
        free(ptr);
    }
    void operator_delete_array(void *ptr)
    {
        free(ptr);
    }

} // namespace os
