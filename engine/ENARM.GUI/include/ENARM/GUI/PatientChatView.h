#pragma once
#include <enarm/gui/AppContext.h>

namespace enarm::gui {

class PatientChatView {
public:
    void render(AppContext& ctx);

private:
    void renderMessages(AppContext& ctx);
    void renderInputArea(AppContext& ctx);
    void checkPendingResponse(AppContext& ctx);
};

} // namespace enarm::gui
