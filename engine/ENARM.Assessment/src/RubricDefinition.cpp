#include <enarm/assessment/RubricDefinition.h>

namespace enarm::assessment {

// ----------------------------------------------------------------
// CriterionScore::toJson
// ----------------------------------------------------------------
nlohmann::json CriterionScore::toJson() const {
    nlohmann::json j;
    j["criterion_id"]          = criterionId;
    j["points_earned"]         = pointsEarned;
    j["max_points"]            = maxPoints;
    j["percentage"]            = percentage;
    j["justification"]         = justification;
    j["elements_identified"]   = elementsIdentified;
    j["elements_missed"]       = elementsMissed;
    return j;
}

// ----------------------------------------------------------------
// DimensionScore::toJson
// ----------------------------------------------------------------
nlohmann::json DimensionScore::toJson() const {
    nlohmann::json j;
    j["dimension"]              = static_cast<int>(dimension);
    j["dimension_name"]         = dimensionToString(dimension);
    j["points_earned"]          = pointsEarned;
    j["max_points"]             = maxPoints;
    j["percentage"]             = percentage;
    j["performance_level"]      = performanceLevel;
    j["time_spent_minutes"]     = timeSpentMinutes;
    j["expected_time_minutes"]  = expectedTimeMinutes;

    nlohmann::json criteria = nlohmann::json::array();
    for (const auto& cs : criterionScores)
        criteria.push_back(cs.toJson());
    j["criterion_scores"] = criteria;

    return j;
}

// ----------------------------------------------------------------
// PerformanceReport::toJson
// ----------------------------------------------------------------
nlohmann::json PerformanceReport::toJson() const {
    nlohmann::json j;
    j["session_id"]               = sessionId;
    j["case_id"]                  = caseId;
    j["student_id"]               = studentId;
    j["total_points_earned"]      = totalPointsEarned;
    j["total_max_points"]         = totalMaxPoints;
    j["global_percentage"]        = globalPercentage;
    j["global_performance_level"] = globalPerformanceLevel;
    j["total_time_minutes"]       = totalTimeMinutes;
    j["time_efficient"]           = timeEfficient;
    j["strengths"]                = strengths;
    j["weaknesses"]               = weaknesses;
    j["recommendations"]          = recommendations;
    j["narrative_feedback"]       = narrativeFeedback;
    j["percentile_rank"]          = percentileRank;
    j["completed_at"]             = completedAt;

    nlohmann::json dims = nlohmann::json::array();
    for (const auto& ds : dimensionScores)
        dims.push_back(ds.toJson());
    j["dimension_scores"] = dims;

    return j;
}

// ----------------------------------------------------------------
// PerformanceReport::fromJson
// ----------------------------------------------------------------
PerformanceReport PerformanceReport::fromJson(const nlohmann::json& j) {
    PerformanceReport r;
    r.sessionId              = j.value("session_id",               "");
    r.caseId                 = j.value("case_id",                  "");
    r.studentId              = j.value("student_id",               "");
    r.totalPointsEarned      = j.value("total_points_earned",      0);
    r.totalMaxPoints         = j.value("total_max_points",         185);
    r.globalPercentage       = j.value("global_percentage",        0.0f);
    r.globalPerformanceLevel = j.value("global_performance_level", "");
    r.totalTimeMinutes       = j.value("total_time_minutes",       0.0f);
    r.timeEfficient          = j.value("time_efficient",           false);
    r.narrativeFeedback      = j.value("narrative_feedback",       "");
    r.percentileRank         = j.value("percentile_rank",          -1.0f);
    r.completedAt            = j.value("completed_at",             "");

    if (j.contains("strengths"))      r.strengths      = j["strengths"].get<std::vector<std::string>>();
    if (j.contains("weaknesses"))     r.weaknesses     = j["weaknesses"].get<std::vector<std::string>>();
    if (j.contains("recommendations"))r.recommendations= j["recommendations"].get<std::vector<std::string>>();

    return r;
}

} // namespace enarm::assessment
