#include <algorithm>
#include "ENARM/GUI/Views/PatientSessionView.h"
#include "ENARM/GUI/AppContext.h"
#include "ENARM/GUI/CaseFindingsProvider.h"
#include "ENARM/GUI/Theme.h"
#include "ENARM/AI/PatientAIController.h"

#include <imgui.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>

namespace ENARM::GUI {

using namespace Theme::Colors;

void PatientSessionView::render(AppContext& ctx) {
    ctx.sessionElapsedSeconds += ImGui::GetIO().DeltaTime;

    renderSessionHeader(ctx);
    ImGui::Spacing();

    float totalW   = ImGui::GetContentRegionAvail().x;
    float scoreW   = 300.0f;
    float contentW = totalW - scoreW - 12.0f;

    // ---- Panel IZQ ----
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BgPanel);
    ImGui::BeginChild("##sessleft", ImVec2(contentW, -1.0f), false);
    {
        renderCompactVitals(ctx);
        ImGui::Spacing();

        // Tabs con look moderno
        ImGui::PushStyleColor(ImGuiCol_Tab,        BgSubtle);
        ImGui::PushStyleColor(ImGuiCol_TabActive,   Accent);
        ImGui::PushStyleColor(ImGuiCol_TabHovered,  AccentHover);
        ImGui::PushStyleColor(ImGuiCol_Text,        TextPrimary);
        ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 8));

        if (ImGui::BeginTabBar("##sesstabs")) {
            if (ImGui::BeginTabItem("Interrogatorio")) {
                ImGui::Spacing();
                renderInterviewTab(ctx);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Exploracion Fisica")) {
                ImGui::Spacing();
                renderPhysicalExamTab(ctx);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Estudios")) {
                ImGui::Spacing();
                renderStudiesTab(ctx);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Diagnostico")) {
                ImGui::Spacing();
                renderDiagnosisTab(ctx);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Tratamiento")) {
                ImGui::Spacing();
                renderTreatmentTab(ctx);
                ImGui::EndTabItem();
            }
            // Sprint 7B: Tab de Guias Clinicas aplicables
            if (ImGui::BeginTabItem("Guias")) {
                RenderGuidelinesTab(ctx);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Seguimiento")) {
                ImGui::Spacing();
                renderFollowUpTab(ctx);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine(0.0f, 12.0f);

    // ---- Panel DER: scoring ----
    ImGui::BeginChild("##sessright", ImVec2(scoreW, -1.0f), false);
    m_scoringPanel.render(ctx);
    ImGui::EndChild();

    if (ctx.showReportModal)
        renderReportModal(ctx);
}

// ================================================================
void PatientSessionView::renderSessionHeader(AppContext& ctx) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float w = ImGui::GetContentRegionAvail().x;
    float h = 60.0f;

    // Fondo con gradiente HERO
    Theme::DrawGradientRectV(dl, pos, ImVec2(pos.x + w, pos.y + h),
        ImGui::ColorConvertFloat4ToU32(HeroTop),
        ImGui::ColorConvertFloat4ToU32(HeroBottom));

    // Sombra suave
    dl->AddRectFilled(ImVec2(pos.x, pos.y + h),
                      ImVec2(pos.x + w, pos.y + h + 4),
                      ImGui::ColorConvertFloat4ToU32(Shadow), 0);

    // Boton Volver (blanco sobre azul)
    ImGui::SetCursorScreenPos(ImVec2(pos.x + 14, pos.y + 14));
    ImGui::PushStyleColor(ImGuiCol_Button,       ImVec4(1,1,1,0.20f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1,1,1,0.35f));
    ImGui::PushStyleColor(ImGuiCol_Text,          TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    if (ImGui::Button("< Volver", ImVec2(90, 32))) {
        ctx.NavigateTo(AppView::CasesList);
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    // Titulo del caso (blanco)
    ImGui::SetCursorScreenPos(ImVec2(pos.x + 116, pos.y + 20));
    ImGui::PushStyleColor(ImGuiCol_Text, TextOnColor);
    ImGui::SetWindowFontScale(1.10f);
    ImGui::Text("%s", ctx.CurrentCaseTitle().c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    // Tiempo (a la derecha)
    int min = static_cast<int>(ctx.sessionElapsedSeconds) / 60;
    int sec = static_cast<int>(ctx.sessionElapsedSeconds) % 60;
    char t[16];
    snprintf(t, sizeof(t), "%02d:%02d", min, sec);

    ImVec2 timeSz = ImGui::CalcTextSize(t);
    dl->AddText(ImVec2(pos.x + w - timeSz.x - 20, pos.y + 22),
                ImGui::ColorConvertFloat4ToU32(TextOnColor), t);

    // Avanzar cursor
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + h + 8));
}

// ================================================================
void PatientSessionView::renderCompactVitals(AppContext& ctx) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float w = ImGui::GetContentRegionAvail().x;
    float h = 76.0f;

    // Fondo card con sombra
    Theme::DrawShadowedRect(dl,
        ImVec2(pos.x, pos.y + 2),
        ImVec2(pos.x + w, pos.y + h + 2),
        12.0f,
        ImGui::ColorConvertFloat4ToU32(ShadowStrong));

    dl->AddRectFilled(pos, ImVec2(pos.x + w, pos.y + h),
                      ImGui::ColorConvertFloat4ToU32(BgPanel), 12.0f);

    // Contenido en columnas
    ImGui::SetCursorScreenPos(ImVec2(pos.x + 12, pos.y + 12));
    ImGui::BeginGroup();
    ImGui::Columns(6, "##vitcols", false);

    struct Vital { const char* label; const char* val; ImVec4 col; };
    const Vital vs[] = {
        {"FC",   "108",    StatusCritical},
        {"PA",   "105/68", StatusWarning},
        {"SpO2", "92%",    StatusWarning},
        {"FR",   "24",     StatusWarning},
        {"T",    "36.2",   StatusNormal},
        {"MAP",  "80",     TextPrimary}
    };

    for (const auto& v : vs) {
        ImGui::PushStyleColor(ImGuiCol_Text, TextDim);
        ImGui::Text("%s", v.label);
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Text, v.col);
        ImGui::SetWindowFontScale(1.35f);
        ImGui::Text("%s", v.val);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();

        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndGroup();

    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + h + 8));
}

// ================================================================
void PatientSessionView::renderInterviewTab(AppContext& ctx) {
    float inputH = 54.0f;
    float chatH  = ImGui::GetContentRegionAvail().y - inputH - 14.0f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, BgSubtle);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
    ImGui::BeginChild("##chatmsgs", ImVec2(-1.0f, chatH), true);

    renderChatMessages(ctx);

    if (ctx.chatScrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        ctx.chatScrollToBottom = false;
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Spacing();
    renderChatInput(ctx);
}

// ================================================================
void PatientSessionView::renderChatMessages(AppContext& ctx) {
    if (ctx.chatMessages.empty()) {
        ImGui::Dummy(ImVec2(0, 30));
        ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
        ImGui::Indent(20.0f);
        ImGui::Text("Inicia la conversacion con el paciente...");
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, TextDim);
        ImGui::Text("Ejemplos:");
        ImGui::BulletText("Buenos dias, cuenteme que le pasa");
        ImGui::BulletText("Desde cuando tiene el dolor");
        ImGui::BulletText("Ha tenido antecedentes similares");
        ImGui::PopStyleColor();
        ImGui::Unindent(20.0f);
        ImGui::PopStyleColor();
        return;
    }

    float chatW      = ImGui::GetContentRegionAvail().x;
    float maxBubbleW = chatW * 0.72f;
    float padding    = 14.0f;
    float bubbleR    = 14.0f;

    for (const auto& msg : ctx.chatMessages) {
        bool isDoctor = msg.isDoctor;

        // Colores pastel: doctor azul accent, paciente gris claro
        ImVec4 bgCol = isDoctor ? Accent : ImVec4(0.90f, 0.92f, 0.96f, 1.0f);
        ImVec4 txCol = isDoctor ? TextOnColor : TextPrimary;
        ImVec4 lbCol = isDoctor ? ImVec4(1,1,1,0.75f) : TextDim;

        ImU32 bgU = ImGui::ColorConvertFloat4ToU32(bgCol);
        ImU32 txU = ImGui::ColorConvertFloat4ToU32(txCol);
        ImU32 lbU = ImGui::ColorConvertFloat4ToU32(lbCol);
        ImU32 shU = ImGui::ColorConvertFloat4ToU32(Shadow);

        ImVec2 textSz = ImGui::CalcTextSize(
            msg.content.c_str(), nullptr, false, maxBubbleW - padding*2.0f);

        float bubbleW = (textSz.x + padding*2.0f < maxBubbleW)
                       ? textSz.x + padding*2.0f : maxBubbleW;
        float bubbleH = textSz.y + padding*2.0f + 18.0f;

        float xOff = isDoctor ? (chatW - bubbleW - 8.0f) : 8.0f;

        ImVec2 cp = ImGui::GetCursorScreenPos();
        cp.x += xOff;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Sombra sutil
        dl->AddRectFilled(ImVec2(cp.x + 2, cp.y + 3),
                          ImVec2(cp.x + bubbleW + 2, cp.y + bubbleH + 3),
                          shU, bubbleR);
        // Burbuja
        dl->AddRectFilled(cp, ImVec2(cp.x + bubbleW, cp.y + bubbleH),
                          bgU, bubbleR);

        // Etiqueta rol
        const char* label = isDoctor ? "Doctor" : "Paciente";
        char labelBuf[128];
        if (!isDoctor && msg.latencyMs > 0) {
            snprintf(labelBuf, sizeof(labelBuf), "Paciente  [%d ms]", msg.latencyMs);
            label = labelBuf;
        }
        dl->AddText(ImVec2(cp.x + padding, cp.y + 6), lbU, label);

        // Texto del mensaje
        ImGui::SetCursorScreenPos(ImVec2(cp.x + padding, cp.y + 22));
        ImGui::PushTextWrapPos(cp.x + bubbleW - padding);
        ImGui::PushStyleColor(ImGuiCol_Text, txCol);
        ImGui::TextWrapped("%s", msg.content.c_str());
        ImGui::PopStyleColor();
        ImGui::PopTextWrapPos();

        ImGui::SetCursorScreenPos(ImVec2(cp.x - xOff, cp.y + bubbleH + 8));
    }

    if (ctx.aiProcessing || ctx.waitingForResponse) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, TextDim);
        ImGui::Indent(16.0f);
        ImGui::Text("   Paciente esta pensando...");
        ImGui::Unindent(16.0f);
        ImGui::PopStyleColor();
    }
}

// ================================================================
void PatientSessionView::renderChatInput(AppContext& ctx) {
    float sendW  = 100.0f;
    float inputW = ImGui::GetContentRegionAvail().x - sendW - 10.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 22.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(16, 12));
    ImGui::PushStyleColor(ImGuiCol_FrameBg,         BgSubtle);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   BgHover);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    BgHover);
    ImGui::PushStyleColor(ImGuiCol_Text,             TextPrimary);

    ImGui::SetNextItemWidth(inputW);
    bool enter = ImGui::InputTextWithHint(
        "##chatinput", "Escribe tu pregunta al paciente...",
        ctx.chatInputBuffer, sizeof(ctx.chatInputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
    ImGui::SameLine(0.0f, 10.0f);

    ImGui::PushStyleColor(ImGuiCol_Button,        Accent);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  AccentHover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   AccentDark);
    ImGui::PushStyleColor(ImGuiCol_Text,           TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 22.0f);

    bool send = ImGui::Button("Enviar", ImVec2(sendW, 44.0f));

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

    if ((enter || send) && std::strlen(ctx.chatInputBuffer) > 0
        && !ctx.aiProcessing && !ctx.waitingForResponse)
    {
        std::string userMsg = ctx.chatInputBuffer;
        ctx.chatInputBuffer[0] = '\0';

        // 1. Agregar mensaje del doctor al chat
        ChatMessage m;
        m.isDoctor = true;
        m.content  = userMsg;
        ctx.chatMessages.push_back(std::move(m));
        ctx.chatScrollToBottom = true;

        // 2. Notificar al assessment engine
        if (ctx.assessmentEngine) {
            ctx.assessmentEngine->onConversationTurn(
                userMsg, "", ctx.sessionElapsedSeconds);
        }

        // 3. Disparar respuesta del paciente en hilo separado
        if (ctx.patient) {
            ctx.waitingForResponse = true;
            ctx.aiProcessing       = true;

            // Copiar puntero a shared para el hilo
            auto patientPtr = ctx.patient.get();
            auto* pending   = &ctx.pendingAIResponse;
            auto* latency   = &ctx.pendingAILatencyMs;
            auto* waiting   = &ctx.waitingForResponse;
            auto* processing= &ctx.aiProcessing;

            std::thread([patientPtr, userMsg, pending, latency, waiting, processing]() {
                auto start = std::chrono::steady_clock::now();
                auto result = patientPtr->Ask(userMsg);
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - start).count();

                if (result.IsOk()) {
                    *pending = result.Value();
                } else {
                    *pending = "[Error de comunicacion con el paciente virtual]";
                }
                *latency    = static_cast<int>(elapsed);
                *processing = false;
                *waiting    = false;
            }).detach();
        } else {
            // Sin AI disponible
            ChatMessage fake;
            fake.isDoctor = false;
            fake.content  = "[Paciente AI no disponible - verificar Ollama]";
            ctx.chatMessages.push_back(std::move(fake));
            ctx.chatScrollToBottom = true;
        }
    }

    // 4. Poll: revisar si llego respuesta del hilo
    if (!ctx.aiProcessing && !ctx.pendingAIResponse.empty()) {
        ChatMessage rep;
        rep.isDoctor  = false;
        rep.content   = ctx.pendingAIResponse;
        rep.latencyMs = ctx.pendingAILatencyMs;
        ctx.chatMessages.push_back(std::move(rep));

        // Notificar al assessment con la respuesta
        if (ctx.assessmentEngine && ctx.chatMessages.size() >= 2) {
            const auto& hist = ctx.chatMessages;
            std::string lastDoctorMsg;
            for (auto it = hist.rbegin(); it != hist.rend(); ++it) {
                if (it->isDoctor) {
                    lastDoctorMsg = it->content;
                    break;
                }
            }
            ctx.assessmentEngine->onConversationTurn(
                lastDoctorMsg, ctx.pendingAIResponse, ctx.sessionElapsedSeconds);
        }

        ctx.pendingAIResponse.clear();
        ctx.pendingAILatencyMs = 0;
        ctx.chatScrollToBottom = true;
    }
}

// ================================================================
static void RenderActionButton(const char* label, bool already,
                                float w, float h, bool& clicked)
{
    ImVec4 bgCol   = already ? StatusNormal : BgSubtle;
    ImVec4 hovCol  = already ? StatusNormal : BgHover;
    ImVec4 txCol   = already ? TextOnColor  : TextPrimary;

    ImGui::PushStyleColor(ImGuiCol_Button,        bgCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  hovCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   bgCol);
    ImGui::PushStyleColor(ImGuiCol_Text,           txCol);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    clicked = ImGui::Button(label, ImVec2(w, h));

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);
}

// ================================================================
void PatientSessionView::renderPhysicalExamTab(AppContext& ctx) {
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Seleccione las areas a explorar:");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    // FIX Sprint 7A.2.5: areas desde Provider (mismas para todos los casos)
    auto exams = CaseFindingsProvider::GetStandardExamAreas();

    for (const auto& e : exams) {
        bool already = ctx.IsExamRequested(e);
        bool click = false;
        RenderActionButton(e.c_str(), already, -1.0f, 34.0f, click);

        if (click && !already) {
            ctx.requestedExams.push_back(e);
            std::string f = ctx.GetPhysicalExamFindings(e);
            ctx.examFindings[e] = f;
            if (ctx.assessmentEngine)
                ctx.assessmentEngine->onPhysicalExamRequested(
                    e, f, ctx.sessionElapsedSeconds);
        }

        if (already && ctx.examFindings.count(e)) {
            ImGui::Indent(16.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, AccentDark);
            ImGui::TextWrapped("%s", ctx.examFindings.at(e).c_str());
            ImGui::PopStyleColor();
            ImGui::Unindent(16.0f);
            ImGui::Spacing();
        }
    }
}

// ================================================================
void PatientSessionView::renderStudiesTab(AppContext& ctx) {
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Solicite los estudios necesarios:");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    // FIX Sprint 7A.2.5: estudios universales del ENARM desde Provider
    auto studies = CaseFindingsProvider::GetStandardStudies();

    ImGui::Columns(2, nullptr, false);
    for (const auto& s : studies) {
        bool already = ctx.IsStudyRequested(s);
        bool click = false;
        RenderActionButton(s.c_str(), already, -1.0f, 30.0f, click);

        if (click && !already) {
            ctx.requestedStudies.push_back(s);
            std::string r = ctx.GetStudyResults(s);
            ctx.studyResults[s] = r;
            if (ctx.assessmentEngine)
                ctx.assessmentEngine->onStudyOrdered(s, r, ctx.sessionElapsedSeconds);
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);

    if (!ctx.studyResults.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, StatusNormal);
        ImGui::Text("Resultados:");
        ImGui::PopStyleColor();
        ImGui::Spacing();
        for (const auto& [s, r] : ctx.studyResults) {
            ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
            ImGui::Text("%s:", s.c_str());
            ImGui::PopStyleColor();
            ImGui::Indent(12.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
            ImGui::TextWrapped("%s", r.c_str());
            ImGui::PopStyleColor();
            ImGui::Unindent(12.0f);
            ImGui::Spacing();
        }
    }
}

// ================================================================
void PatientSessionView::renderDiagnosisTab(AppContext& ctx) {
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Formule su diagnostico clinico:");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    if (m_diagSubmitted) {
        ImGui::PushStyleColor(ImGuiCol_Text, StatusNormal);
        ImGui::Text("Diagnostico enviado: %s", m_diagBuffer);
        ImGui::PopStyleColor();
        ImGui::Spacing();
        if (ImGui::Button("Modificar diagnostico"))
            m_diagSubmitted = false;
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_FrameBg,        BgSubtle);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  BgHover);
    ImGui::PushStyleColor(ImGuiCol_Text,           TextPrimary);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImGui::Text("Diagnostico principal:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##diag", m_diagBuffer, sizeof(m_diagBuffer));

    ImGui::Spacing();
    ImGui::Text("Justificacion:");
    ImGui::InputTextMultiline("##just", m_justBuffer, sizeof(m_justBuffer),
                              ImVec2(-1.0f, 80.0f));

    ImGui::Spacing();
    ImGui::Text("Diagnosticos diferenciales:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 100.0f);
    ImGui::InputText("##diff", m_diffBuffer, sizeof(m_diffBuffer));

    ImGui::SameLine();
    if (ImGui::Button("Agregar") && strlen(m_diffBuffer) > 0) {
        ctx.differentials.push_back(m_diffBuffer);
        m_diffBuffer[0] = '\0';
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    for (const auto& d : ctx.differentials) {
        ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
        ImGui::BulletText("%s", d.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,        Accent);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  AccentHover);
    ImGui::PushStyleColor(ImGuiCol_Text,          TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

    if (ImGui::Button("Confirmar Diagnostico", ImVec2(-1.0f, 42.0f))
        && strlen(m_diagBuffer) > 0) {
        ctx.submittedDiagnosis     = m_diagBuffer;
        ctx.diagnosisJustification = m_justBuffer;
        m_diagSubmitted            = true;
        if (ctx.assessmentEngine)
            ctx.assessmentEngine->onDiagnosisSubmitted(
                m_diagBuffer, m_justBuffer, ctx.sessionElapsedSeconds);
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}

// ================================================================
void PatientSessionView::renderTreatmentTab(AppContext& ctx) {
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Indique el plan terapeutico:");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_FrameBg,        BgSubtle);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  BgHover);
    ImGui::PushStyleColor(ImGuiCol_Text,           TextPrimary);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImGui::Text("Medicamento / Intervencion:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##med", m_medBuffer, sizeof(m_medBuffer));

    ImGui::Text("Dosis / Via / Frecuencia:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##dose", m_doseBuffer, sizeof(m_doseBuffer));

    ImGui::Text("Justificacion:");
    ImGui::InputTextMultiline("##rat", m_rationaleBuffer, sizeof(m_rationaleBuffer),
                              ImVec2(-1.0f, 64.0f));

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,        StatusNormal);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0.25f, 0.80f, 0.50f, 1));
    ImGui::PushStyleColor(ImGuiCol_Text,          TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

    if (ImGui::Button("Agregar al Plan", ImVec2(-1.0f, 40.0f))
        && strlen(m_medBuffer) > 0) {
        TreatmentEntry entry;
        entry.medication = m_medBuffer;
        entry.dose       = m_doseBuffer;
        entry.rationale  = m_rationaleBuffer;
        ctx.treatments.push_back(entry);

        if (ctx.assessmentEngine)
            ctx.assessmentEngine->onTreatmentOrdered(
                m_medBuffer, m_doseBuffer, m_rationaleBuffer,
                ctx.sessionElapsedSeconds);

        m_medBuffer[0] = m_doseBuffer[0] = m_rationaleBuffer[0] = '\0';
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    if (!ctx.treatments.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, StatusNormal);
        ImGui::Text("Plan Terapeutico:");
        ImGui::PopStyleColor();
        ImGui::Spacing();
        for (size_t i = 0; i < ctx.treatments.size(); ++i) {
            const auto& t = ctx.treatments[i];
            ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
            ImGui::Text("%zu. %s", i+1, t.medication.c_str());
            ImGui::PopStyleColor();
            ImGui::Indent(12.0f);
            if (!t.dose.empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
                ImGui::Text("Dosis: %s", t.dose.c_str());
                ImGui::PopStyleColor();
            }
            if (!t.rationale.empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
                ImGui::TextWrapped("Razon: %s", t.rationale.c_str());
                ImGui::PopStyleColor();
            }
            ImGui::Unindent(12.0f);
            ImGui::Spacing();
        }
    }
}

// ================================================================
void PatientSessionView::renderFollowUpTab(AppContext& ctx) {
    ImGui::PushStyleColor(ImGuiCol_Text, TextSecondary);
    ImGui::Text("Plan de seguimiento y monitoreo:");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_FrameBg,        BgSubtle);
    ImGui::PushStyleColor(ImGuiCol_Text,           TextPrimary);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::InputTextMultiline("##fu", m_fuBuffer, sizeof(m_fuBuffer),
                              ImVec2(-1.0f, 110.0f));
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.30f, 0.65f, 0.85f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  AccentHover);
    ImGui::PushStyleColor(ImGuiCol_Text,          TextOnColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

    if (ImGui::Button("Registrar Seguimiento", ImVec2(-1.0f, 40.0f))
        && strlen(m_fuBuffer) > 0) {
        ctx.followUpActions.push_back(m_fuBuffer);
        if (ctx.assessmentEngine)
            ctx.assessmentEngine->onFollowUpAction(
                m_fuBuffer, "", ctx.sessionElapsedSeconds);
        m_fuBuffer[0] = '\0';
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    if (!ctx.followUpActions.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, Accent);
        ImGui::Text("Registrado:");
        ImGui::PopStyleColor();
        for (const auto& a : ctx.followUpActions) {
            ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
            ImGui::BulletText("%s", a.c_str());
            ImGui::PopStyleColor();
        }
    }
}

// ================================================================
void PatientSessionView::renderReportModal(AppContext& ctx) {
    if (!ctx.lastReport) {
        ctx.showReportModal = false;
        return;
    }

    auto& report = *ctx.lastReport;

    ImGui::OpenPopup("Reporte Final de Evaluacion");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(760.0f, 640.0f), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_PopupBg, BgPanel);
    ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);

    if (ImGui::BeginPopupModal("Reporte Final de Evaluacion",
                                &ctx.showReportModal,
                                ImGuiWindowFlags_NoResize))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, TextTitle);
        ImGui::SetWindowFontScale(1.25f);
        ImGui::Text("Resultado Final - %s", report.caseId.c_str());
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::Spacing();

        ImVec4 scoreCol = (report.globalPercentage >= 80.0f)
            ? StatusNormal
            : (report.globalPercentage >= 60.0f) ? StatusWarning : StatusCritical;

        ImGui::PushStyleColor(ImGuiCol_Text, scoreCol);
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%.1f%%  (%d/%d pts)  -  %s",
            report.globalPercentage, report.totalPointsEarned,
            report.totalMaxPoints, report.globalPerformanceLevel.c_str());
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::Spacing();

        // Chart barras
        {
            static const char* dimLabels[] = {
                "Interrog.","Explor.","Estudios",
                "Diagnostico","Tratamiento","Seguimiento"
            };
            static const ImU32 barColors[] = {
                IM_COL32( 99,102,241,255), IM_COL32(168, 85,247,255),
                IM_COL32(236, 72,153,255), IM_COL32(251,146, 60,255),
                IM_COL32( 34,197, 94,255), IM_COL32( 56,189,248,255)
            };

            float chartH = 170.0f;
            float chartW = ImGui::GetContentRegionAvail().x;
            ImVec2 pos   = ImGui::GetCursorScreenPos();
            ImDrawList* dl = ImGui::GetWindowDrawList();

            dl->AddRectFilled(pos, ImVec2(pos.x + chartW, pos.y + chartH),
                              ImGui::ColorConvertFloat4ToU32(BgSubtle), 8.0f);

            float y70 = pos.y + chartH * (1.0f - 0.70f);
            dl->AddLine(ImVec2(pos.x, y70), ImVec2(pos.x + chartW, y70),
                        ImGui::ColorConvertFloat4ToU32(StatusCritical), 1.5f);
            dl->AddText(ImVec2(pos.x + 6, y70 - 16),
                        ImGui::ColorConvertFloat4ToU32(StatusCritical), "70%");

            int   n     = (int)report.dimensionScores.size();
            float barW  = (n > 0) ? (chartW - 8.0f) / (float)n : chartW;

            for (int i = 0; i < n && i < 6; ++i) {
                float pct = report.dimensionScores[i].percentage / 100.0f;
                float bx  = pos.x + 4.0f + i * barW;
                float bh  = (chartH - 26.0f) * pct;
                float by  = pos.y + chartH - 22.0f - bh;

                dl->AddRectFilled(
                    ImVec2(bx + 5.0f, by),
                    ImVec2(bx + barW - 5.0f, pos.y + chartH - 22.0f),
                    barColors[i], 4.0f);

                dl->AddText(
                    ImVec2(bx + 2.0f, pos.y + chartH - 20.0f),
                    ImGui::ColorConvertFloat4ToU32(TextSecondary),
                    dimLabels[i]);
            }

            ImGui::Dummy(ImVec2(chartW, chartH + 8.0f));
        }

        ImGui::Spacing();
        ImGui::Columns(2, nullptr, false);
        if (!report.strengths.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, StatusNormal);
            ImGui::Text("Fortalezas:");
            ImGui::PopStyleColor();
            for (const auto& s : report.strengths) {
                ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
                ImGui::BulletText("%s", s.c_str());
                ImGui::PopStyleColor();
            }
        }
        ImGui::NextColumn();
        if (!report.weaknesses.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, StatusCritical);
            ImGui::Text("Areas de mejora:");
            ImGui::PopStyleColor();
            for (const auto& w : report.weaknesses) {
                ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
                ImGui::BulletText("%s", w.c_str());
                ImGui::PopStyleColor();
            }
        }
        ImGui::Columns(1);

        if (!report.recommendations.empty()) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, Accent);
            ImGui::Text("Recomendaciones:");
            ImGui::PopStyleColor();
            for (size_t i = 0; i < report.recommendations.size(); ++i) {
                ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
                ImGui::BulletText("%zu. %s", i+1, report.recommendations[i].c_str());
                ImGui::PopStyleColor();
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, BgSubtle);
        ImGui::PushStyleColor(ImGuiCol_Text, TextPrimary);
        if (ImGui::Button("Cerrar", ImVec2(120.0f, 38.0f)))
            ctx.showReportModal = false;
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Accent);
        ImGui::PushStyleColor(ImGuiCol_Text, TextOnColor);
        if (ImGui::Button("Dashboard", ImVec2(140.0f, 38.0f))) {
            ctx.showReportModal = false;
            ctx.NavigateTo(AppView::Dashboard);
        }
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, StatusNormal);
        ImGui::PushStyleColor(ImGuiCol_Text, TextOnColor);
        if (ImGui::Button("Nueva Sesion", ImVec2(140.0f, 38.0f))) {
            ctx.showReportModal = false;
            ctx.NavigateTo(AppView::CasesList);
        }
        ImGui::PopStyleColor(2);

        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(2);
}



// ============================================================
// Sprint 7B: RenderGuidelinesTab
// Muestra GPCs aplicables a la especialidad del caso activo
// ============================================================
void PatientSessionView::RenderGuidelinesTab(AppContext& ctx) {
    // Carga lazy
    if (ctx.sessionGuidelines.empty() && ctx.activeCase.has_value()) {
        ctx.LoadSessionGuidelines();
    }

    if (ctx.sessionGuidelines.empty()) {
        ImGui::TextDisabled("No hay guias clinicas disponibles para esta especialidad:");
        if (ctx.activeCase.has_value()) {
            ImGui::TextDisabled("  %s", ctx.activeCase->specialty.c_str());
        }
        ImGui::Spacing();
        ImGui::TextWrapped("Sugerencia: revisa el catalogo completo desde Menu > Guias Clinicas (Ctrl+G)");
        return;
    }

    ImGui::TextColored(ImVec4(0.4f, 0.6f, 0.9f, 1.0f),
        "Guias aplicables a %s (%zu disponibles)",
        ctx.activeCase.has_value() ? ctx.activeCase->specialty.c_str() : "?",
        ctx.sessionGuidelines.size());

    // ---- BARRA DE FILTROS ----
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), "Filtros:");
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.15f, 0.72f, 0.37f, 1.0f));
        ImGui::Checkbox("Primarias", &ctx.guideFilterPrimary);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.88f, 0.58f, 0.08f, 1.0f));
        ImGui::Checkbox("Secundarias", &ctx.guideFilterSecondary);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.55f, 0.60f, 0.68f, 1.0f));
        ImGui::Checkbox("Terciarias", &ctx.guideFilterTertiary);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(20.0f, 0.0f));
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.90f, 0.35f, 0.35f, 1.0f));
        ImGui::Checkbox("Solo pendientes", &ctx.guideFilterOnlyPending);
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Oculta consultadas con >=100%% adherencia");
        }
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(20.0f, 0.0f));
        ImGui::SameLine();

        ImGui::Text("Orden:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(140.0f);
        const char* sortOpts[] = { "Relevancia", "ID (A-Z)", "Adherencia %", "Titulo" };
        ImGui::Combo("##sortMode", &ctx.guideSortMode, sortOpts, IM_ARRAYSIZE(sortOpts));
        ImGui::Spacing();
    }

    // ---- ADHERENCIA ----
    ctx.EvaluateAdherence();
    const double totalPct = Assessment::GuidelineAdherenceScorer::TotalPercent(ctx.currentAdherence);
    const int totalPts = Assessment::GuidelineAdherenceScorer::TotalPoints(ctx.currentAdherence);
    const int maxPts = Assessment::GuidelineAdherenceScorer::MaxPoints(ctx.currentAdherence);

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.2f, 1.0f), "ADHERENCIA A GPCs");
    ImVec4 barCol = (totalPct >= 75.0f) ? ImVec4(0.2f, 0.7f, 0.3f, 1.0f) :
                    (totalPct >= 50.0f) ? ImVec4(0.9f, 0.7f, 0.2f, 1.0f) :
                                           ImVec4(0.9f, 0.3f, 0.3f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, barCol);
    char adhLabel[64];
    snprintf(adhLabel, sizeof(adhLabel), "%.0f%% (%d / %d pts)", totalPct, totalPts, maxPts);
    ImGui::ProgressBar((float)(totalPct / 100.0), ImVec2(-1, 22), adhLabel);
    ImGui::PopStyleColor();

    int totalMatched = 0, totalRecs = 0;
    for (const auto& a : ctx.currentAdherence) {
        totalMatched += a.matchedRecommendations;
        totalRecs += a.totalRecommendations;
    }
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
        "%zu consultadas | %d/%d recomendaciones cumplidas",
        ctx.consultedGuidelineIds.size(), totalMatched, totalRecs);

    ImGui::Separator();
    ImGui::Spacing();

    // ---- VISTA FILTRADA + ORDENADA ----
    std::vector<const Data::Guideline*> view;
    view.reserve(ctx.sessionGuidelines.size());
    for (const auto& g : ctx.sessionGuidelines) {
        auto itR = ctx.sessionGuidelineRelevance.find(g.id);
        int rel = (itR != ctx.sessionGuidelineRelevance.end()) ? itR->second : 0;
        if (rel == 1 && !ctx.guideFilterPrimary)   continue;
        if (rel == 2 && !ctx.guideFilterSecondary) continue;
        if (rel == 3 && !ctx.guideFilterTertiary)  continue;
        if (rel == 0 && !ctx.guideFilterPrimary)   continue;

        if (ctx.guideFilterOnlyPending) {
            const bool consulted = (ctx.consultedGuidelineIds.count(g.id) > 0);
            double pct = 0.0;
            for (const auto& a : ctx.currentAdherence) {
                if (a.guidelineId == g.id) { pct = a.adherencePercent; break; }
            }
            if (consulted && pct >= 100.0) continue;
        }
        view.push_back(&g);
    }

    auto getRel = [&](const Data::Guideline* p) -> int {
        auto it = ctx.sessionGuidelineRelevance.find(p->id);
        return (it != ctx.sessionGuidelineRelevance.end()) ? it->second : 99;
    };
    auto getAdh = [&](const Data::Guideline* p) -> double {
        for (const auto& a : ctx.currentAdherence) {
            if (a.guidelineId == p->id) return a.adherencePercent;
        }
        return 0.0;
    };
    switch (ctx.guideSortMode) {
        case 0: std::sort(view.begin(), view.end(),
                [&](const Data::Guideline* a, const Data::Guideline* b) {
                    int ra = getRel(a), rb = getRel(b);
                    if (ra != rb) return ra < rb;
                    return a->id < b->id;
                }); break;
        case 1: std::sort(view.begin(), view.end(),
                [](const Data::Guideline* a, const Data::Guideline* b) {
                    return a->id < b->id;
                }); break;
        case 2: std::sort(view.begin(), view.end(),
                [&](const Data::Guideline* a, const Data::Guideline* b) {
                    return getAdh(a) > getAdh(b);
                }); break;
        case 3: std::sort(view.begin(), view.end(),
                [](const Data::Guideline* a, const Data::Guideline* b) {
                    return a->title < b->title;
                }); break;
    }

    size_t viewConsulted = 0;
    for (auto* p : view) {
        if (ctx.consultedGuidelineIds.count(p->id) > 0) ++viewConsulted;
    }
    ImGui::TextColored(ImVec4(0.70f, 0.80f, 0.95f, 1.0f),
        "Mostrando %zu de %zu   |   Consultadas en vista: %zu",
        view.size(), ctx.sessionGuidelines.size(), viewConsulted);
    ImGui::Spacing();

    // ---- LISTA DE GPCs ----
    for (const auto* gp : view) {
        const auto& g = *gp;
        const bool consulted = ctx.consultedGuidelineIds.count(g.id) > 0;

        double gpcPct = 0.0;
        for (const auto& adhR : ctx.currentAdherence) {
            if (adhR.guidelineId == g.id) { gpcPct = adhR.adherencePercent; break; }
        }
        char pctStr[16];
        snprintf(pctStr, sizeof(pctStr), " [%.0f%%]", gpcPct);

        // Badge de relevancia
        auto itRel = ctx.sessionGuidelineRelevance.find(g.id);
        if (itRel != ctx.sessionGuidelineRelevance.end()) {
            int rel = itRel->second;
            const char* lbl = (rel == 1) ? " PRIMARIA "   :
                              (rel == 2) ? " SECUNDARIA " :
                                            " TERCIARIA ";
            ImVec4 col = (rel == 1) ? ImVec4(0.15f,0.72f,0.37f,1.0f) :
                         (rel == 2) ? ImVec4(0.88f,0.58f,0.08f,1.0f) :
                                      ImVec4(0.42f,0.46f,0.55f,1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button,        col);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  col);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
            ImGui::SmallButton((std::string(lbl) + "##rel_" + g.id).c_str());
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(4);
            ImGui::SameLine();
        }

        std::string header = (consulted ? "[OK] " : "[  ] ") + g.id + " - " + g.title + pctStr;
        if (ImGui::CollapsingHeader(header.c_str())) {
            if (!consulted) {
                ctx.consultedGuidelineIds.insert(g.id);
            }

            ImGui::Indent(20.0f);

            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                "%s | %s | %d",
                g.organization.value_or("").c_str(),
                g.specialty.value_or("").c_str(),
                g.year.value_or(0));

            if (g.summary.has_value() && !g.summary->empty()) {
                ImGui::Spacing();
                ImGui::TextWrapped("%s", g.summary->c_str());
            }

            if (!g.recommendations.empty()) {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.3f, 0.7f, 0.3f, 1.0f),
                    "Recomendaciones clave (%zu):", g.recommendations.size());
                for (const auto& r : g.recommendations) {
                    ImGui::Bullet();
                    ImGui::SameLine();
                    ImGui::TextWrapped("[%s] %s", r.level.c_str(), r.text.c_str());
                }
            }

            ImGui::Spacing();
            std::string btnId = "Ver detalle completo##" + g.id;
            if (ImGui::Button(btnId.c_str())) {
                ctx.activeGuideline = g;
                ctx.NavigateTo(AppView::GuidelineDetail);
            }

            ImGui::Unindent(20.0f);
            ImGui::Spacing();
        }
    }
}
} // namespace ENARM::GUI

