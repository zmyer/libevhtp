set(LIBEVHTP_SOURCES "${LIBEVHTP_SOURCES} evhtp/evhtp_heap.c")

install(FILES
	evhtp_heap.h
	DESTINATION ${INCLUDE_INSTALL_DIR}/evhtp/heap)

