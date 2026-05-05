#pragma once

#include <drogon/HttpController.h>
#include <drogon/utils/coroutine.h>

using namespace drogon;

class ProductManagerController : public HttpController<ProductManagerController>
{
public:
    METHOD_LIST_BEGIN

    // 1. Create product (Manager only)
    ADD_METHOD_TO(ProductManagerController::create,
                  "/api/products",
                  Post,
                  "AuthNFilter", "AuthZFilter");

    // 2. Get product by ID (Public)
    ADD_METHOD_TO(ProductManagerController::getById,
                  "/api/products/{1}",
                  Get);

    // 3. List products (Public)
    ADD_METHOD_TO(ProductManagerController::list,
                  "/api/products",
                  Get);

    // 4. Search products (Public)
    ADD_METHOD_TO(ProductManagerController::search,
                  "/api/products/search",
                  Get);

    // 5. Update product (Manager only)
    ADD_METHOD_TO(ProductManagerController::update,
                  "/api/products/{1}",
                  Put,
                  "AuthNFilter", "AuthZFilter");

    // 6. Soft delete product (Manager only)
    ADD_METHOD_TO(ProductManagerController::remove,
                  "/api/products/{1}",
                  Delete,
                  "AuthNFilter", "AuthZFilter");

    METHOD_LIST_END

public:
    // POST /api/products
    Task<HttpResponsePtr> create(const HttpRequestPtr req);

    // GET /api/products/{id}
    Task<HttpResponsePtr> getById(const HttpRequestPtr req,
                                  int64_t id);

    // GET /api/products
    Task<HttpResponsePtr> list(const HttpRequestPtr req);

    // GET /api/products/search
    Task<HttpResponsePtr> search(const HttpRequestPtr req);

    // PUT /api/products/{id}
    Task<HttpResponsePtr> update(const HttpRequestPtr req,
                                 int64_t id);

    // DELETE /api/products/{id}
    Task<HttpResponsePtr> remove(const HttpRequestPtr req,
                                 int64_t id);
};