#include <limine.h>

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;


//TODO: add some requests