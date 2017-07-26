include(ExternalProject)

ExternalProject_Add(glm
    #if you want use newest glm please remove -DGIT_TAG flag
    DOWNLOAD_COMMAND ${CMAKE_COMMAND} -DGIT_REPO=https://github.com/g-truc/glm -DGIT_TAG=27f8e5b0a161b49f06ff259ca783dabd7a9498fd -DGIT_REPO_DIR=glm -P ${PROJECT_SOURCE_DIR}/cmake/git_sparse_checkout.cmake
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    # Disable configure, build and install step
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    )

# Specify include dir
ExternalProject_Get_Property(glm source_dir)
set(GLM_INCLUDE_DIRS ${source_dir}/../ PARENT_SCOPE)
