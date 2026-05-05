(function () {
    const NAV_TREE = [
        {
            name: "manager",
            items: [
                { label: "Products", href: "/manager/products.html" },
                { label: "báo cáo sale", href: "/manager/sale-report.html" },
                { label: "Quản lý tài khoản nhân viên", href: "/manager/accounts.html" }
            ]
        },
        {
            name: "staff",
            items: [
                { label: "Make Order", href: "/" },
                { label: "Order CRUD", href: "/staff/order-crud.html" }
            ]
        },
        {
            name: "đăng nhập/đăng ký",
            items: [
                { label: "đăng nhập", href: "/auth/login.html" },
                { label: "đăng ký", href: "/auth/signup.html" }
            ]
        }
    ];

    /**
     * Kiểm tra trạng thái authentication từ server (HttpOnly cookie)
     */
    async function checkAuth() {
        try {
            const response = await fetch('/auth/me', { method: 'GET' });
            if (response.ok) {
                return await response.json();
            }
        } catch (e) {
            console.error("Auth check failed", e);
        }
        return { authenticated: false };
    }

    /**
     * Xử lý đăng xuất qua POST /auth/logout
     */
    async function handleLogout() {
        try {
            const response = await fetch('/auth/logout', { 
                method: 'POST',
                headers: { 'Content-Type': 'application/json' }
            });
            const result = await response.json();
            if (result.success) {
                window.location.href = "/auth/login.html";
            }
        } catch (error) {
            console.error("Logout failed:", error);
        }
    }

    /**
     * Tạo Navbar với cụm nút Auth được thiết kế đẹp hơn
     */
    function createNavbar(authState) {
        const nav = document.createElement("div");
        nav.className = "tp-navbar";

        // Cụm bên trái: Logo và Tên thương hiệu
        const left = document.createElement("div");
        left.className = "tp-navbar-left";

        const logo = document.createElement("img");
        logo.className = "tp-logo";
        logo.src = "/thimpos.png";
        logo.alt = "ThimPOS";

        const title = document.createElement("span");
        title.className = "tp-navbar-title";
        title.textContent = "ThimPOS";

        left.appendChild(logo);
        left.appendChild(title);
        nav.appendChild(left);

        // Cụm bên phải: Nút Auth
        const right = document.createElement("div");
        right.className = "tp-navbar-right";

        if (authState.authenticated) {
            // Container thông tin user và nút logout
            const userContainer = document.createElement("div");
            userContainer.className = "tp-auth-container";

            const userBadge = document.createElement("span");
            userBadge.className = "tp-user-badge";
            userBadge.innerHTML = `<i class="tp-icon-user"></i> ${authState.username}`;

            const logoutBtn = document.createElement("button");
            logoutBtn.className = "tp-btn tp-btn-danger";
            logoutBtn.textContent = "Đăng xuất";
            logoutBtn.onclick = handleLogout;

            userContainer.appendChild(userBadge);
            userContainer.appendChild(logoutBtn);
            right.appendChild(userContainer);
        } else {
            // Cụm Login/Signup
            const authContainer = document.createElement("div");
            authContainer.className = "tp-auth-container";

            const loginLink = document.createElement("a");
            loginLink.className = "tp-btn tp-btn-ghost";
            loginLink.href = "/auth/login.html";
            loginLink.textContent = "Đăng nhập";

            const signupLink = document.createElement("a");
            signupLink.className = "tp-btn tp-btn-primary";
            signupLink.href = "/auth/signup.html";
            signupLink.textContent = "Đăng ký";

            authContainer.appendChild(loginLink);
            authContainer.appendChild(signupLink);
            right.appendChild(authContainer);
        }

        nav.appendChild(right);
        return nav;
    }

    /**
     * Tạo Sidebar danh mục
     */
    function createSidebar() {
        const sidebar = document.createElement("div");
        sidebar.className = "tp-sidebar";

        const brand = document.createElement("div");
        brand.className = "tp-brand";
        brand.textContent = "ThimPOS";
        sidebar.appendChild(brand);

        NAV_TREE.forEach(folder => {
            const folderDiv = document.createElement("div");
            folderDiv.className = "tp-folder";

            const title = document.createElement("div");
            title.className = "tp-folder-title";
            title.textContent = folder.name;
            folderDiv.appendChild(title);

            folder.items.forEach(item => {
                const a = document.createElement("a");
                a.className = "tp-link";
                a.textContent = item.label;
                a.href = item.href;

                if (window.location.pathname === item.href) {
                    a.classList.add("active");
                }
                folderDiv.appendChild(a);
            });
            sidebar.appendChild(folderDiv);
        });

        return sidebar;
    }

    /**
     * Hàm inject layout tổng thể
     */
    async function injectLayout() {
        const authState = await checkAuth();
        const body = document.body;

        const navbar = createNavbar(authState);
        const sidebar = createSidebar();

        const main = document.createElement("div");
        main.className = "tp-main";

        // Wrap nội dung cũ của trang vào trong tp-main
        while (body.firstChild) {
            main.appendChild(body.firstChild);
        }

        body.appendChild(navbar);
        body.appendChild(sidebar);
        body.appendChild(main);
    }

    // Check-list:
    // 1. [x] Đầy đủ hàm logic (checkAuth, handleLogout, createNavbar, createSidebar, injectLayout)
    // 2. [x] UI cập nhật với các class: tp-btn, tp-btn-primary, tp-btn-ghost, tp-btn-danger
    // 3. [x] Đã thêm container bao bọc cụm nút để căn chỉnh (tp-auth-container)
    // 4. [x] Gán đúng endpoint /auth/me và /auth/logout (POST) theo contract

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", injectLayout);
    } else {
        injectLayout();
    }
})();