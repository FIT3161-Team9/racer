## Vision

The aim of this project is to create a simulation-based game that provides entertainment, as well as an accurate
physics simulation of vehicles moving through a 2D track.

## High Level Process Plan

The project will be completed in sprints. Each sprint will be 2 weeks long. The high level tasks for each sprint will be:

1. Sprint Retrospective

   1. Scrum Master will run a retro where each team member will summarise their contribution
      and have an opportunity to provide feedback/concerns on the process

2. Sprint Planning

   3. Backlog Population: Stories are selected from the "Project Backlog" to be put in the "Sprint Backlog"
   4. Estimation: Stories are estimated based on story point system (see below)
   5. Assignment: Up to two project members are assigned each ticket to be responsible for the deliver of that ticket

3. Sprinting

   1. Tickets are moved from the "Sprint Backlog" to "Doing", and any number of project members collaboratively work
      on the ticket.
   2. When a ticket is ready for review, a PR is created (and one of the project members who worked on the ticket
      is assiged to the PR) and the ticket is moved from "Doing" to "Review"
   3. At least one other member of the team reviews the ticket
      1. If they are happy, they approve it and the project member assigned the ticket merges the PR - the ticket is then moved
         from "Review" to "Done"
      2. If they are unhappy, the provide clear and constructive feedback in the form of a PR review
      3. The project member assigned to the PR must then make changes and resolve the feedback, before requesting another review
         on the PR and the process loops back to `3.3.1`

## Stakeholders

- Main Client
  - Alan Dorin
- Licensing
  - FIT3161 Team 9
- End Users
  - Anyone who wants to download and play the game

## Sprint Planning Methodology

### Sprint Planning

0. Create project stories, (if after inception, should only be from sprint retro)
   1. Tasks that will/might improve the project's maintainability
   2. Example: CI, management process updates, build environment changes, documentation needs, etc.
1. Meet with product owner to generate stories
   1. Assign level of priority to each story
   2. Ensure each story card card has acceptance criterias
2. Break each story into tasks
   1. Identify what tasks will be worked on for the current sprint
   2. Estimate the amonut of work for each task - call them `story points`
   3. Identify risks associated with the ticket, and enter them into a risk register
   4. Add up the total `story points` for each epic
   5. Move tasks into current sprint backlog
3. Assigns tasks to be done to current sprint.
   1. Estimate the amount of time that the team can work on this project.
   2. Ensure that everyone has something they **can** work on for the sprint.
   3. Priorities blocking tickets first, especially if they block other high-priority tickets.
   4. If incomplete tickets from the previous sprint are not complete
      1. If it blocks tasks from the current sprint, move it to the current sprint
      2. If the ticket priority drops for some reason (client or otherwise), return ticket to backlog.
4. Assign owners to each of the tasks in the sprint.
   1. Each team member will provide an estimated number of hours of availability
   2. The scrum master will go through each ticket and assign up to two team members that will be responsible for delivering that ticket
   3. Team members will check the number of story points assigned to them and ensure it lines up with their availability during the sprint
   4. If a team member has too many/few story points from the sprint tickets are re-allocated as needed.

Story points table for reference:
| Story points | Description | Time estimate |
|--------------|-----------------------------------------------------------------------------|---------------|
| 1 | Answering a question, quick investigation | < 30 mins |
| 2 | Copy text / very minor code edits | < 1 hour |
| 3 | Extensive docs / minor code edits | < 3 hours |
| 5 | Substantial work, like a sub-feature | < 1 day |
| 8 | Substantial work, with potentially unknown technology | < 2 days |
| 13 | Too big, like a full feature. Break down the task or convert it to an epic. | Full sprint |

### During Sprints

For each individual team member:

1. Work on an assigned ticket.
2. Once the ticket is complete, put it through the review process
   1. Ticket reviews aren't tasks.
   2. Review other peoples' tasks in your own time.
   3. All tests should be run as part of the review process.

### Sprint Retrospective

Sprint retros are to be done immediately before the **next sprint planning** session. This helps in keeping the `what needs to be improved` ideas fresh. If anything needs to be improved, we create tickets accordingly. The sprint retrospective will be a meeting ran by the scrum master and will
give all memebers of the team the opportunity to reflect on how the previous sprint, as well as on
the project management process as a whole. Furthermore, during the sprint retro, any risks that
occurred during the previous sprint that weren't in the risk register will be analysed and added
to the register.

## Team Management

The team is following a modified version of the Agile methodology. As such, there are three main roles being used: product owner, scrum master, and developer.

- Product Owner: Alan Dorin
- Scrum Master: Harrison Marshall
- Developer: All members in the team
