#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <iostream>

struct Packet {
    std::string src_ip;
    std::string dst_ip;
    int port;
    int size;
};

std::vector<Packet> loadPackets(const char* db_path) {
    std::vector<Packet> packets;
    sqlite3* db;
    if (sqlite3_open(db_path, &db) != SQLITE_OK) return packets;

    const char* sql = "SELECT src_ip, dst_ip, dst_port, size FROM packets LIMIT 100;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return packets;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        packets.push_back({
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            sqlite3_column_int(stmt, 2),
            sqlite3_column_int(stmt, 3)
        });
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return packets;
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "NetWatch GUI", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    std::vector<Packet> packets;
    bool showTable = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("NetWatch Dashboard");
        if (ImGui::Button("Carregar Pacotes")) {
            packets = loadPackets("packets.db");
            showTable = true;
        }

        if (showTable) {
            ImGui::Separator();
            ImGui::Columns(4, "Packets");
            ImGui::Text("Origem"); ImGui::NextColumn();
            ImGui::Text("Destino"); ImGui::NextColumn();
            ImGui::Text("Porta"); ImGui::NextColumn();
            ImGui::Text("Tamanho"); ImGui::NextColumn();
            ImGui::Separator();

            for (auto& p : packets) {
                ImGui::Text("%s", p.src_ip.c_str()); ImGui::NextColumn();
                ImGui::Text("%s", p.dst_ip.c_str()); ImGui::NextColumn();
                ImGui::Text("%d", p.port); ImGui::NextColumn();
                ImGui::Text("%d", p.size); ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }

        ImGui::End();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
